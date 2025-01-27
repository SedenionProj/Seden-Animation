#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "src/renderer.hpp"
#include "src/object/components.hpp"
#include "src/logger.h"
#include "src/object/object.hpp"

namespace Seden {
	static void GLAPIENTRY MessageCallback(GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam)
	{
		fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			type, severity, message);
	}

	Renderer::Renderer(Window& window)
		:m_window(window)
	{
		gladLoadGL();
		
		initImgui();

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallback, 0);

		m_polygonData.vao= std::make_unique<VertexArray>();
		m_polygonData.ibo = std::make_unique<IndexBuffer>();
		m_polygonData.vbo = std::make_unique<VertexBuffer>();
		m_polygonData.shader = std::make_unique<Shader>();
		m_polygonData.shader->createShader(baseVertexShader, baseFragmentShader);
		m_polygonData.vao->addVertexBuffer(*m_polygonData.vbo, VertexArrayLayout({
			3, // position
			3, // color
		}));

		m_shader = std::make_unique<Shader>();
		m_shader->createShader(letterVertexShader, letterFragmentShader);

		m_font = std::make_unique<Font>();
	}

	Renderer::~Renderer()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Renderer::setCamera(std::shared_ptr<PerspectiveCamera> camera)
	{
		m_camera = camera;
	}

	void Renderer::initImgui()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;


		ImGui_ImplGlfw_InitForOpenGL(m_window.getWindowPtr(), true);
		ImGui_ImplOpenGL3_Init("#version 330");
	}

	void Renderer::beginFrame()
	{
		glfwPollEvents();
		glClearColor(0.1f,0.1f,0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// imgui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (Comp::PolygonMesh::hasVertexCountChanged) {
			m_polygonData.indicesList.clear();
			m_polygonData.indicesList.reserve(Comp::PolygonMesh::totalVertexCount);
			m_polygonData.verticesList.resize(Comp::PolygonMesh::totalVertexCount);
		}
	}

	void Renderer::endFrame()
	{
		if (Comp::PolygonMesh::totalVertexCount) {
			if (Comp::PolygonMesh::hasVertexCountChanged) {
				m_polygonData.ibo->setData(m_polygonData.indicesList.size(), m_polygonData.indicesList.data());
				Comp::PolygonMesh::hasVertexCountChanged = false;
			}
			m_polygonData.vbo->setData(Comp::PolygonMesh::totalVertexCount * sizeof(Comp::PolygonMesh::Vertex), m_polygonData.verticesList.data());

			m_polygonData.vertexOffset = 0;
			m_polygonData.shader->Bind();
			m_polygonData.shader->setMat4("view", m_camera->getView());
			m_polygonData.shader->setMat4("proj", m_camera->getProjection());
		
			m_polygonData.vao->bind();
			glDrawElements(GL_TRIANGLES, m_polygonData.ibo->getCount(), GL_UNSIGNED_INT, nullptr);
		}
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(m_window.getWindowPtr());

		
	}

	void Renderer::drawDebugGui() {
		ImGui::Begin("debug");
		ImGui::Text("ms %f", m_stats.frameTimeElapsed);
		ImGui::Text("fps %f", 1.f/m_stats.frameTimeElapsed);
		ImGui::Text("animation count %d", m_stats.animationCount);
		ImGui::Text("object alive count %d", m_stats.objectAliveCount);
		ImGui::End();
	}

	void Renderer::drawConvexPolygon(Comp::Transform& transform, Comp::PolygonMesh& mesh)
	{
		// todo: skip if Vertex has not changed, multiple batches, reserve the right amount of indices
		const size_t n = mesh.getVertexCount();

		if (Comp::PolygonMesh::hasVertexCountChanged) {
			for (int i = 1; i < n - 1; i++) {
				m_polygonData.indicesList.emplace_back(m_polygonData.vertexOffset);
				m_polygonData.indicesList.emplace_back(m_polygonData.vertexOffset + i);
				m_polygonData.indicesList.emplace_back(m_polygonData.vertexOffset + i + 1);
			}
		}


		for (int i = 0; i < n; i++) {
			const auto& meshVertex = mesh.getVertex(i);
			auto& vertex = m_polygonData.verticesList[m_polygonData.vertexOffset + i];
			vertex.position = transform.getTransform() * glm::vec4(meshVertex.position, 1);
			vertex.color = meshVertex.color;
		}

		m_polygonData.vertexOffset += n;
	}

	struct LetterVertex {
		glm::vec3 position;
		glm::vec2 texCoord;
	};

	void Renderer::drawSimpleText(Comp::Transform& transform, Comp::Text& text)
	{
		float x = 0, y = 0;
		uint32_t i = 0;
		float scale = 100;
		float lineSpace = 30;
		float lineSkip = 0;
		float center = 0;
		stbtt_aligned_quad q;
		char* letter = (char*)text.getText().c_str();
		while (*letter) {
			if (*letter == '\n') {
				lineSkip += lineSpace;
				center = x;
			}
			else if (*letter >= 32 && *letter < 128) {
				stbtt_GetBakedQuad(m_font->cdata, m_font->texResolution, m_font->texResolution, *letter - 32, &x, &y, &q, 1);
				//createQuad(q, glm::vec2(-center / scale, lineSkip / scale), i++);

				glm::vec4 pos = glm::vec4(-center / scale, lineSkip / scale,0,1);
				LetterVertex vertices[4] = {
					{ transform.getTransform() * (glm::vec4(q.x0, q.y0, 0.f, 0.f) / scale + pos), {q.s0, q.t0} },
					{ transform.getTransform() * (glm::vec4(q.x1, q.y0, 0.f, 0.f)/scale + pos), {q.s1, q.t0} },
					{ transform.getTransform() * (glm::vec4(q.x1, q.y1, 0.f, 0.f)/scale + pos), {q.s1, q.t1} },
					{ transform.getTransform() * (glm::vec4(q.x0, q.y1, 0.f, 0.f)/scale + pos), {q.s0, q.t1} }
				};

				VertexBuffer vb(4 * sizeof(LetterVertex), (void*)vertices);

				VertexArray va;
				va.addVertexBuffer(vb, VertexArrayLayout({
					3, // position
					2, // texCoord
					}));
				m_shader->Bind();
				m_shader->setMat4("view", m_camera->getView());
				m_shader->setMat4("proj", m_camera->getProjection());
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_font->ftex);
				m_shader->setInt("uTexture", 0);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			}
			else {
				stbtt_GetBakedQuad(m_font->cdata, m_font->texResolution, m_font->texResolution, 127 - 32, &x, &y, &q, 1);
				//createQuad(q, glm::vec2(-center / scale, lineSkip / scale), i++);
			}
			letter++;
		}
	}
}
