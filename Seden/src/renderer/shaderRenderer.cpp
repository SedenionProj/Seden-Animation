#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "src/window.hpp"
#include "src/renderer/shaderRenderer.hpp"
#include "src/object/object.hpp"

namespace Seden {

	void ShaderRenderer::init(Window* window)
	{
		m_window = window;

		initImgui();
		glEnable(GL_DEBUG_OUTPUT);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glViewport(0, 0, m_window->getWidth(), m_window->getHeight());
		float vertices[] = {
			-1,  1,
			 1,  1,
			 1, -1,
			-1, -1
		};
		m_vao = std::make_unique<VertexArray>();
		m_vbo = std::make_unique<VertexBuffer>(sizeof(vertices), static_cast<void*>(vertices));
		m_vao->addVertexBuffer(*m_vbo, { 2 });

		std::vector<glm::vec4> colBuf(m_window->getWidth() * m_window->getHeight(), glm::vec4(1.0));
		m_colorSSBO = std::make_unique<ShaderStorageBuffer>(colBuf.size() * sizeof(glm::vec4), colBuf.data());

		std::vector<float> arrBuf(1024, 0.f);
		m_arraySSBO = std::make_unique<ShaderStorageBuffer>(1024 * sizeof(float), arrBuf.data());

		m_colorSSBO->bind(0);
		m_arraySSBO->bind(1);

		// screen
		m_screenShader.createShader(screenVertexShader, screenFragmentShader);
		m_framebuffer = std::make_unique<Framebuffer>(m_window->getWidth(), m_window->getHeight());

		// text
		m_letterShader = std::make_unique<Shader>();
		m_letterShader->createShader(letterVertexShader, letterFragmentShader);
		m_font = std::make_unique<Font>();
	}
	void ShaderRenderer::beginFrame()
	{
		glfwPollEvents();
		m_framebuffer->bind();
		// imgui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}
	void ShaderRenderer::endFrame()
	{
		// record and draw on framebuffer
		m_window->saveFrame();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// draw on screen
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		m_screenShader.bind();
		m_vao->bind();
		glBindTexture(GL_TEXTURE_2D, m_framebuffer->getTexture());

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		m_window->swapBuffers();
	}



	void ShaderRenderer::draw(float time, float dt)
	{
		ImGui::Begin("debug");
		m_shader.bind();
		
		m_shader.setVec2("iResolution",glm::vec2(m_window->getWidth(), m_window->getHeight()));
		m_shader.setVec2("iTime", {time, dt});
		ImGui::Text("time : %f", time);
		ImGui::SliderInt("passes number", &m_renderPassCount, 0, 50);
		if (ImGui::Button("recompile")) {
			if (!m_shader.m_vertexPath.empty()) {
				m_shader.createShaderPath(m_shader.m_vertexPath, m_shader.m_fragmentPath);
			}
			else {
				DEBUG_ERROR("Shaders path missing");
			}
		}
		ImGui::SeparatorText("variables");
		for (auto& u : m_uniforms) {
			std::string format = "%." + std::to_string(static_cast<int>(u.range.z)) + "f";

			switch (u.dataType) {
			case Seden::ShaderDataType::BOOL:
				m_shader.setBool(u.name, *(bool*)u.data);
				ImGui::Checkbox(u.name.c_str(), (bool*)u.data);
				break;
			case Seden::ShaderDataType::INT:
				m_shader.setInt(u.name, *(int*)u.data);
				ImGui::SliderInt(u.name.c_str(), (int*)u.data, u.range.x, u.range.y, format.c_str());
				break;
			case Seden::ShaderDataType::FLOAT:
				m_shader.setFloat(u.name, *(float*)u.data);
				ImGui::SliderFloat(u.name.c_str(), (float*)u.data, u.range.x, u.range.y, format.c_str());
				break;
			case Seden::ShaderDataType::VEC2:
				m_shader.setVec2(u.name, *(glm::vec2*)u.data);
				ImGui::SliderFloat2(u.name.c_str(), (float*)u.data, u.range.x, u.range.y, format.c_str());
				break;
			case Seden::ShaderDataType::VEC3:
				m_shader.setVec3(u.name, *(glm::vec3*)u.data);
				ImGui::SliderFloat3(u.name.c_str(), (float*)u.data, u.range.x, u.range.y, format.c_str());
				break;
			case Seden::ShaderDataType::VEC4:
				m_shader.setVec4(u.name, *(glm::vec4*)u.data);
				ImGui::SliderFloat4(u.name.c_str(), (float*)u.data, u.range.x, u.range.y, format.c_str());
				break;
			case Seden::ShaderDataType::MAT2:
				break;
			case Seden::ShaderDataType::MAT3:
				break;
			case Seden::ShaderDataType::MAT4:
				m_shader.setMat4(u.name, *(glm::mat4*)u.data);
				break;
			default:
				break;
			}
		}
		ImGui::End();

		for (int i = 0; i < m_renderPassCount; i++) {
			m_shader.setInt("iPass", i);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			
		}
	}

	struct LetterVertex {
		glm::vec4 color;
		glm::vec3 position;
		glm::vec2 texCoord;
	};

	void ShaderRenderer::drawText(Comp::Transform& transform, Comp::GroupObjects& letters, Comp::Text& text) {
		float x = 0, y = 0;
		uint32_t i = 0;
		float scale = m_font->fontSize/(text.getScale()*0.01);
		float lineSpace = 1;
		float lineSkip = 0;
		float center = 0;
		stbtt_aligned_quad q;
		char* letter = (char*)text.getText().c_str();
        auto it = letters.begin();
		while (*letter) {
			
			if (*letter == '\n') {
				lineSkip += lineSpace;
				center = x;
			}
			else if (*letter == ' ') {
				stbtt_GetBakedQuad(m_font->cdata, m_font->texResolution, m_font->texResolution, *letter - 32, &x, &y, &q, 1);
			} else if (*letter >= 32 && *letter < 128) {
				stbtt_GetBakedQuad(m_font->cdata, m_font->texResolution, m_font->texResolution, *letter - 32, &x, &y, &q, 1);
				
				glm::mat4 model = transform.getTransform()*(*it)->get<Comp::Transform>().getTransform();
				

				float midx = (q.x1-q.x0) / 2.f;
				float midy = (q.y1-q.y0) / 2.f;
				glm::vec4 pos = glm::vec4(-center + q.x0 + midx, lineSkip + q.y0 + midy, 0, 0);
				auto& col = (*it)->get<Comp::Color>();
				LetterVertex vertices[4] = {
					{ col.m_color, (model * glm::vec4(-midx, -midy+m_font->fontSize, 0.f, scale) +pos) / scale, {q.s0, q.t0} },
					{ col.m_color, (model * glm::vec4(midx, -midy +m_font->fontSize, 0.f, scale) + pos) / scale, {q.s1, q.t0} },
					{ col.m_color, (model * glm::vec4(midx,  midy +m_font->fontSize, 0.f, scale) + pos) / scale, {q.s1, q.t1} },
					{ col.m_color, (model * glm::vec4(-midx,  midy+m_font->fontSize, 0.f,scale) + pos) / scale, {q.s0, q.t1} }
				};

				VertexBuffer vb(4 * sizeof(LetterVertex), (void*)vertices);

				VertexArray va;
				va.addVertexBuffer(vb, VertexArrayLayout({
					4, // color
					3, // position
					2, // texCoord
					}));
				m_letterShader->bind();
				m_letterShader->setMat4("view", glm::mat4(1));
				m_letterShader->setMat4("proj", glm::ortho(-m_window->getAspectRatio(), m_window->getAspectRatio(), 1.f, -1.f, -1.f, 1.f));
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_font->ftex);
				m_letterShader->setInt("uTexture", 0);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
				it++;
			}
			else {
				stbtt_GetBakedQuad(m_font->cdata, m_font->texResolution, m_font->texResolution, 127 - 32, &x, &y, &q, 1);
				//createQuad(q, glm::vec2(-center / scale, lineSkip / scale), i++);
			}
			letter++;
		}
	}

	void ShaderRenderer::initImgui()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;


		ImGui_ImplGlfw_InitForOpenGL(m_window->getWindowPtr(), true);
		ImGui_ImplOpenGL3_Init("#version 330");
	}
}