#include "src/application.hpp"
#include "src/scene.hpp"
#include "src/object.hpp"
#include "src/components.hpp"

class TestApp : public Seden::Application {
public:
	void animation() override {
		using namespace Seden;

		auto o = Quad::create(scene);

		


		scene.wait(1000);
	}

private:
	Seden::Scene scene;
};

int main() {
	try {
		TestApp app;
		app.start();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
}