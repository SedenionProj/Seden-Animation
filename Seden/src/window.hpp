#pragma once
#include <GLFW/glfw3.h>
#include <string>

namespace Seden {
	class Window {
	public:
		Window(uint32_t width = 1280, uint32_t height = 720, const char* name = "application")
			: m_width(width), m_height(height) {
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			m_window = glfwCreateWindow(m_width, m_height, name, nullptr, nullptr);
			glfwMakeContextCurrent(m_window);
			glfwSwapInterval(0);
		}

		~Window() {
			stopRecording();
			glfwDestroyWindow(m_window);
		}

		void setVsync(bool val) {
			glfwSwapInterval(val);
		}

		uint32_t getFrameRate() {
			return m_frameRate;
		}
		bool isRunning() {
			return !glfwWindowShouldClose(m_window);
		}

		bool isRecording() {
			return m_isRecording;
		}

		void startRecording(const std::string& path, int fps) {
			m_image = std::make_unique<int[]>(m_width * m_height);
			m_frameRate = fps;
			std::string cmd = "ffmpeg -r " + std::to_string(fps) +
				" -f rawvideo -pix_fmt rgba -s " + std::to_string(m_width) + "x" + std::to_string(m_height) + " -i - " +
				"-threads 0 -preset fast -y -pix_fmt yuv420p -crf 21 -vf vflip " + path + ".mp4";

			ffmpeg = _popen(cmd.c_str(), "wb");
			m_isRecording = true;
		}
		void saveFrame() {
			if (m_isRecording) {
				glReadPixels(0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, m_image.get());
				fwrite(m_image.get(), sizeof(int) * m_width * m_height, 1, ffmpeg);
			}
		}
		void stopRecording() {
			if (m_isRecording) {
				m_isRecording = false;
				_pclose(ffmpeg);
			}
		}

		void swapBuffers() {
			glfwSwapBuffers(m_window);
		}

		GLFWwindow* getWindowPtr() { return m_window; }

	private:
		uint32_t m_frameRate;
		uint32_t m_width;
		uint32_t m_height;
		bool m_isRecording = false;
		std::unique_ptr<int[]> m_image;
		FILE* ffmpeg = nullptr;
		GLFWwindow* m_window;
	};
}