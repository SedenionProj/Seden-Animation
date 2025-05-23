#pragma once
#include <GLFW/glfw3.h>

namespace Seden {
	class Window {
	public:
		Window();

		void create(uint32_t width, uint32_t height, const char* name);

		~Window();

		void setVsync(bool val);

		uint32_t getFrameRate();
		bool isRunning();

		bool isRecording();

		void startRecording(const std::string& path, int fps);
		void saveFrame();
		void stopRecording();

		void swapBuffers();
		GLFWwindow* getWindowPtr();

		uint32_t getWidth() { return m_width; }
		uint32_t getHeight() { return m_height; }
		float getAspectRatio() { return (float) m_width / m_height; }

	private:
		uint32_t m_frameRate = 0;
		uint32_t m_width = 0;
		uint32_t m_height = 0;
		bool m_isRecording = false;
		std::unique_ptr<int[]> m_image;
		FILE* ffmpeg = nullptr;
		GLFWwindow* m_window;
	};
}