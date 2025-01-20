#pragma once
#include <GLFW/glfw3.h>

namespace Seden {
	class Window {
	public:
		Window(uint32_t width = 1280, uint32_t height = 720, const char* name = "application") {
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			window = glfwCreateWindow(width, height, name, nullptr, nullptr);
			glfwMakeContextCurrent(window);
			glfwSwapInterval(0);
		}

		~Window() {
			glfwDestroyWindow(window);
		}

		bool isRunning() {
			return !glfwWindowShouldClose(window);
		}

		GLFWwindow* getWindowPtr() { return window; }

	private:
		GLFWwindow* window;
	};
}