#include "src/application.h"

int main() {
	try {
		Seden::Application app;
		app.start();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
}