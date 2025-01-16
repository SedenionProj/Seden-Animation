#define SEDEN_VERBOSE

#include <glm/gtc/matrix_transform.hpp>

#include "src/application.hpp"
#include "src/object/object.hpp"
#include "src/object/components.hpp"
#include "src/animation/curve.hpp"

// todo: animation, camera, text, batching, math/utils, custom shader

class TestApp : public Seden::Application {
public:
	void animation() override {
		using namespace Seden;

		auto o1 = Quad::create(scene);
		auto o2 = Quad::create(scene);
		auto o3 = Quad::create(scene);
		o1->get<PolygonMesh>().setColor({ 0.7,0.5,0.7 });
		o2->get<PolygonMesh>().setColor({ 0.0,0.5,0.7 });
		o3->get<PolygonMesh>().setColor({ 0.0,0.0,0.7 });

		//scene.animate<glm::vec3>({.var = &o1->get<PolygonMesh>().getVertex(0).position, .to = glm::vec3(0.1) , .curve = new EaseIn(10), .time = 2.f});
		//scene.animate<glm::vec3>({.var = &o1->get<PolygonMesh>().getVertex(0).position, .to = glm::vec3(0.1) , .curve = new EaseInOut(1), .time = 3.5f});
		//scene.wait(0.5);
		//scene.animate<glm::vec3>({.var = &o2->get<PolygonMesh>().getVertex(1).color, .to = glm::vec3(0,1,0)});

		//scene.animGroup<MoveTo>({ o1 }, glm::vec3(0., 1, 0));
		scene.animate({ .anim = new MoveTo(o1, glm::vec3(1, 0, 0)), .curve = new EaseInOut(1), .time = 5 });
		//scene.animate({ .anim = new MoveTo(o1, glm::vec3(0, 1, 0)), .curve = new EaseInOut(1), .time = 2 });

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