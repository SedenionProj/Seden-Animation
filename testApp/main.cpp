#include "exampleScenes/mandelbulb/mandelbulb.h"

int main() {
	try {
		Application app;
		app.startScene(new TestShader);
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
}