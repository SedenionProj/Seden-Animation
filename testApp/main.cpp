#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "src/application.hpp"
#include "src/object/object.hpp"
#include "src/object/components.hpp"
#include "src/animation/curve.hpp"

/*todo: 
	- quad, point, custom shader, spline objects
	- replace "std::shared_ptr" and "new"
	- lookat, orthographic camera
	- animation system: group, new animators
	- rethink scene system
*/ 

using namespace Seden;
using namespace glm;

class TestApp : public Seden::Application {

public:
	void animation() override {
		auto cam = PerspectiveCamera::create();
		scene.setCamera(cam);

		Comp::GroupObjects texts;
		scene.block();

		for (int y = 0; y < 2; y++) {
			auto t = texts.addObject(Text::create("hash("+std::to_string(y)+") = "+std::to_string(myHash(y)), vec3(0, y, 3)));
			//t->get<Comp::GroupObjects>().getObject(10)->get<Comp::Transform>().rotate(glm::quat(0.5, 0, 0, 1));
			scene.animAttach(new Wave(t->get<Comp::GroupObjects>().getObject(10)), 3);
		}
		//scene.anim(new TranslateBy(cam, { 1.5,0,-5 }), 10);

		scene.unBlock();


		DEBUG_MSG("fin");
	}

	/*
	beginScene(caml);

	endScene();
	*/

private:
	float myHash(float seed) {
		seed = glm::fract(sin(seed));
		return seed;
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