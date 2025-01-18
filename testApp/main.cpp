#define SEDEN_VERBOSE

#include <glm/gtc/matrix_transform.hpp>

#include "src/application.hpp"
#include "src/object/object.hpp"
#include "src/object/components.hpp"
#include "src/animation/curve.hpp"

/*todo: 
	- text, batching, custom shader, non blocking "wait", thread waiter/blocker
	- replace "std::shared_ptr" and "new"
	- lookat, orthographic camera
	- remake animation system and function : attach? + group? + variable?, add new animations
*/ 

class TestApp : public Seden::Application {
public:
	void animation() override {
		using namespace Seden;
		auto cam = PerspectiveCamera::create(scene);
		scene.setCamera(cam);

		auto o1 = Quad::create(scene);

		//scene.animate<glm::vec3>({.var = &o1->get<PolygonMesh>().getVertex(0).position, .to = glm::vec3(0.1) , .curve = new EaseInOut(5), .time = 2.f});
		//scene.animate<glm::vec3>({.var = &o1->get<PolygonMesh>().getVertex(0).position, .to = glm::vec3(0.1) , .curve = new EaseInOut(1), .time = 3.5f});
		//scene.wait(0.5);
		//scene.animate<glm::vec3>({.var = &o1->get<PolygonMesh>().getVertex(1).color, .to = glm::vec3(0,1,0)});

		//scene.animGroup<TranslateBy>({ o1, o2, o3 }, glm::vec3(0., 0, -5));
		//scene.animate({ .anim = new TranslateBy(o1, glm::vec3(1, 0, 0)), .curve = new EaseInOut(5), .time = 2 });
		//scene.wait(1);
		//scene.animate({ .anim = new RotateTo(o1, glm::angleAxis(3.1415f/2, glm::vec3(0,0,1))), .curve = new EaseInOut(5), .time = 2 });
		//scene.animate({ .anim = new MoveTo(o1, glm::vec3(1, 0, 0)), .curve = new EaseInOut(5), .time = 2 });
		//scene.animate({ .anim = new ScaleTo(o1, glm::vec3(0.5,0.1,0.3)), .curve = new EaseInOut(5), .time = 2 });

		scene.animAttach(new TranslateBy(o1, glm::vec3(10, 0, 10)));

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