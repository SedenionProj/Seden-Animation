#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "src/application.hpp"
#include "src/object/object.hpp"
#include "src/object/components.hpp"
#include "src/animation/curve.hpp"

/*todo: 
	- text, quad, point, custom shader, spline objects
	- replace "std::shared_ptr" and "new"
	- lookat, orthographic camera
	- animation system: group, new animators
	- rethink scene system
*/ 

class TestApp : public Seden::Application {
public:
	void animation() override {
		using namespace Seden;
		using namespace glm;
		auto cam = PerspectiveCamera::create(scene);
		scene.setCamera(cam);

		scene.block();
		auto text = SimpleText::create(scene, "test");
		auto quad = Quad::create(scene);
		auto quad2 = Quad::create(scene);
		quad->get<Comp::Transform>().setPosition({ 1, 1, 0 });
		
		scene.unBlock();
		//scene.wait(3);
		scene.anim(new MoveTo(text, vec3(1, 1,2)));

		DEBUG_MSG("fin");
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