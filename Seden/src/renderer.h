#pragma once
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuizmo.h>

#include "src/window.h"
#include "src/applicationInfo.h"
#include "src/plugin_api/GUIwindow.h"

namespace Seden {

	void SetupImGuiFonts() {
		ImGuiIO& io = ImGui::GetIO();

		io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\arial.ttf", 15.0f);

		io.Fonts->Build();
	}

	class Renderer {
	public:
		Renderer(Window& window, AppInfo& appInfo)
			: window(window), m_appInfo(appInfo) {
			gladLoadGL();

			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

			SetupImGuiFonts();

			ImGui_ImplGlfw_InitForOpenGL(window.getWindowPtr(), true);
			ImGui_ImplOpenGL3_Init("#version 330");
		}

		void beginFrame() {
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			m_appInfo.isContinuousRefresh ? glfwPollEvents() : glfwWaitEvents();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();

			ImGui::NewFrame();
			ImGuizmo::BeginFrame();
			ImGui::DockSpaceOverViewport();
		}

		void endFrame() {
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			glfwSwapBuffers(window.getWindowPtr());
		}

		void drawFrame() {
			for (auto scene : m_appInfo.getScenes()) {
				scene->draw();
			}

			auto& windows = m_appInfo.getWindows();

			for (size_t i = 0; i < windows.size(); ) {
				GuiWindow* window = windows[i];
				if (window->show_window) {
					window->draw();
					++i;
				}
				else {
					delete window;
					windows.erase(windows.begin() + i);
				}
			}

		}

	private:
		AppInfo& m_appInfo;
		Window& window;
	};
}