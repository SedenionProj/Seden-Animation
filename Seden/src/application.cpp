#include "src/application.hpp"

namespace Seden{
	Application::~Application() {
		glfwTerminate();
	}

	void Application::start() {
		std::jthread anim(std::bind(&Application::animation, this));
		scene.startAnimationLoop();
		anim.join();
		//loop.detach();
	}
}