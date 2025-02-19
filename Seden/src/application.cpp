#include "src/application.hpp"
#include "src/object/object.hpp"
namespace Seden{
	Scene* s_scene;

	Application::~Application() {
		glfwTerminate();
	}

	void Application::start() {
		ApplicationInfo appInfo;
		appInfo.resolution = { 1920,1080 };
		appInfo.optionsFlags |= ApplicationInfo::RECORD;
		//appInfo.optionsFlags |= ApplicationInfo::VSYNC;
		
		m_width = appInfo.resolution.x;
		m_height = appInfo.resolution.y;

		m_window.create(m_width, m_height, "application");
		scene.init(&m_window);

		if (appInfo.optionsFlags & ApplicationInfo::VSYNC)
			m_window.setVsync(true);
		
		if (appInfo.optionsFlags & ApplicationInfo::RECORD)
			m_window.startRecording(appInfo.path, appInfo.frameRate);



		s_scene = &scene;
		std::jthread anim(std::bind(&Application::animation, this));
		scene.startAnimationLoop();
		anim.join();
	}
}