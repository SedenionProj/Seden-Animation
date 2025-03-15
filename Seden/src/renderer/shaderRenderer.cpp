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
		float vertices[] = {
			-1,  1,
			 1,  1,
			 1, -1,
			-1, -1
		};
		m_vao = std::make_unique<VertexArray>();
		m_vbo = std::make_unique<VertexBuffer>(sizeof(vertices), static_cast<void*>(vertices));
		m_vao->addVertexBuffer(*m_vbo, { 2 });

		std::vector<glm::vec4> colBuf(m_window->getWidth() * m_window->getHeight(), glm::vec4(0.0));
		m_ssbo = std::make_unique<ShaderStorageBuffer>(colBuf.size() * sizeof(glm::vec4), colBuf.data());
	}
	void ShaderRenderer::beginFrame()
	{
		glfwPollEvents();

		glViewport(0, 0, m_window->getWidth(), m_window->getHeight());

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

		m_window->swapBuffers();
	}



	void ShaderRenderer::draw()
	{
		ImGui::Begin("debug");
		m_shader.bind();
		m_ssbo->bind();
		ImGui::SliderFloat2("range", glm::value_ptr(m_range), -100, 100);
		if (ImGui::Button("recompile")) {
			if (!m_shader.m_vertexPath.empty()) {
				m_shader.createShaderPath(m_shader.m_vertexPath, m_shader.m_fragmentPath);
			}
			else {
				DEBUG_ERROR("Shaders path missing");
			}
		}
		ImGui::Separator();
		for (auto& u : m_uniforms) {
			
			switch (u.dataType) {
			case Seden::ShaderDataType::BOOL:
				m_shader.setBool(u.name, *(bool*)u.data);
				ImGui::Checkbox(u.name.c_str(), (bool*)u.data);
				break;
			case Seden::ShaderDataType::INT:
				m_shader.setInt(u.name, *(int*)u.data);
				ImGui::SliderInt(u.name.c_str(), (int*)u.data, m_range.x, m_range.y);
				break;
			case Seden::ShaderDataType::FLOAT:
				m_shader.setFloat(u.name, *(float*)u.data);
				ImGui::SliderFloat(u.name.c_str(), (float*)u.data, m_range.x, m_range.y);
				break;
			case Seden::ShaderDataType::VEC2:
				m_shader.setVec2(u.name, *(glm::vec2*)u.data);
				ImGui::SliderFloat2(u.name.c_str(), (float*)u.data, m_range.x, m_range.y);
				break;
			case Seden::ShaderDataType::VEC3:
				m_shader.setVec3(u.name, *(glm::vec3*)u.data);
				ImGui::SliderFloat3(u.name.c_str(), (float*)u.data, m_range.x, m_range.y);
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

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
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