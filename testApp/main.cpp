#define SEDEN_VERBOSE

#include <glm/gtc/matrix_transform.hpp>
#include "src/application.hpp"
#include "src/scene.hpp"
#include "src/object.hpp"
#include "src/components.hpp"
#include "src/animation/curve.hpp"

// todo: transform, animation, camera, batching

class TestApp : public Seden::Application {
public:
	void animation() override {
		using namespace Seden;

		auto o = Quad::create(scene);
		o->get<PolygonMesh>().setColor({ 1,0.7,1 });
		o->get<PolygonMesh>().setColorGradient({ {1,0,0}, {0,1,0}, {0,0,1} });

		//auto poly = Polygon::create(scene, {
		//	{{-0.5f, -0.5f,1} , {0,0,1}}, 
		//	{{0.5f, -0.5f,1} , {0,1,0}},
		//	{{0.5f, 0.5f,1}, {1,0,1}},
		//	{{0,1,0}, {1,0,0}},
		//	{{-0.5f, 0.5f,1} , {1,1,0}}
		//	});
		//
		//poly->get<PolygonMesh>().setColor({1,0,0});

		scene.animate(&o->get<Transform>().getTransform(),
			glm::translate(o->get<Transform>().getTransform(), glm::vec3(1.,1.,0)),
			1,EaseInOut(0.1));
		scene.wait(1);
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