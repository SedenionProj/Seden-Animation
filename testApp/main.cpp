#define SEDEN_VERBOSE

#include <glm/gtc/matrix_transform.hpp>

#include "src/application.hpp"
#include "src/object/object.hpp"
#include "src/components.hpp"
#include "src/animation/curve.hpp"

// todo: animation, group animation, camera, text, batching, math/utils, custom shader

class TestApp : public Seden::Application {
public:
	void animation() override {
		using namespace Seden;

		auto o = Quad::create(scene);
		o->get<PolygonMesh>().setColor({ 0.7,0.5,0.7 });

		scene.animate<glm::vec3>({.var = &o->get<PolygonMesh>().getVertex(0).position, .to = glm::vec3(1) , .curve = EaseIn(10), .time = 10});
		scene.wait(0.5);
		scene.animate<glm::vec3>({.var = &o->get<PolygonMesh>().getVertex(1).color, .to = glm::vec3(0,1,0)});

		scene.wait(100);
	}
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