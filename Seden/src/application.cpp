#include "src/application.hpp"
#include "src/object/object.hpp"
namespace Seden{
	Scene* s_scene;

	Application::~Application() {
		glfwTerminate();
	}

	void Application::start() {
		s_scene = &scene;
		std::jthread anim(std::bind(&Application::animation, this));
		scene.startAnimationLoop();
		anim.join();
	}
}