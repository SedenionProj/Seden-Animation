#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "src/window.hpp"
#include "src/renderer/shaderRenderer.hpp"
#include "src/logger.h"

namespace Seden {

	void ShaderRenderer::init(Window* window)
	{
		m_window = window;

		initImgui();
		glEnable(GL_DEBUG_OUTPUT);
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

		m_screenShader.createShader(screenVertexShader, screenFragmentShader);
		m_framebuffer = std::make_unique<Framebuffer>(m_window->getWidth(), m_window->getHeight());
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
		m_colorSSBO->bind();
		m_arraySSBO->bind();
		m_shader.setVec2("iResolution",glm::vec2(m_window->getWidth(), m_window->getHeight()));
		m_shader.setVec2("iTime", {time, dt});
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