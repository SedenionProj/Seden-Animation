#include "src/application.hpp"

namespace Seden{
	Application::~Application() {
		glfwTerminate();
	}

	void Application::start() {
		animation();
	}
}