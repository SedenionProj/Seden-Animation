#define SEDEN_VERBOSE

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
	- remake animation system and function : attach? + group? + variable?, add new animations
*/ 

class TestApp : public Seden::Application {
public:
	void animation() override {
		using namespace Seden;
		auto cam = PerspectiveCamera::create(scene);
		scene.setCamera(cam);

		scene.block();
		auto poly = ConvexPolygon::create(scene, {
			{{-0.5f, -0.5f,1} , {0,0,1}},
			{{0.5f, -0.5f,1} , {0,1,0}},
			{{0.5f, 0.5f,1}, {1,0,1}},
			{{0,1,1}, {1,0,0}},
			{{-0.5f, 0.5f,1} , {1,1,0}}
			});

		
		auto poly2 = ConvexPolygon::create(scene, {
			{{-0.5f, -0.5f,1} , {0,0,1}},
			{{0.5f, -0.5f,1} , {0,1,0}},
			{{0.5f, 0.5f,1}, {1,0,1}},
			{{0,1,1}, {1,0,0}},
			{{-0.5f, 0.5f,1} , {1,1,0}}
			});
		
		
		scene.animate({ .anim = new MoveTo(poly2, glm::vec3(1,1,0)), .curve = new EaseInOut(5), .time = 2 });
        scene.animate({ .anim = new RotateTo(poly2, glm::quat(glm::vec3(0.f, 0.f, 3.14f))), .curve = new EaseInOut(5), .time = 2 });
		poly2->get<Comp::Transform>().setPosition(glm::vec3(1,1,0));
		scene.unBlock();

		scene.block();
		for (int i = -20; i < 20; i++) {
			for (int j = -20; j < 20; j++) {
				
				auto o3 = Quad::create(scene);
				o3->get<Comp::Transform>().setScale(glm::vec3(0.05));
				o3->get<Comp::Transform>().translate(glm::vec3(i, j, 0));
				o3->get<Comp::Transform>().setScale(glm::vec3(0.03));
				scene.animate({ .anim = new MoveTo(o3, glm::vec3(-1, -1, 0)), .curve = new EaseInOut(5), .time = 1. });
				
			}
		}
		scene.unBlock();
		
		scene.block();
		auto o1 = Quad::create(scene);
		auto o2 = Quad::create(scene);
		
		scene.animate({ .anim = new TranslateBy(o1, glm::vec3(1, 0, 0)), .curve = new EaseInOut(5), .time = 2 });
		scene.unBlock();
		
		scene.wait(1);
		
		scene.block();
		scene.animate({ .anim = new RotateTo(o1, glm::angleAxis(3.1415f / 2, glm::vec3(0,0,1))), .curve = new EaseInOut(5), .time = 2 });
		
		
		scene.animate({ .anim = new ScaleTo(o1, glm::vec3(0.5,0.1,0.3)), .curve = new EaseInOut(5), .time = 2 });
		scene.animGroup<TranslateBy>({o1,o2}, glm::vec3(0., 0, -1));
		scene.animAttach(new TranslateBy(o2, glm::vec3(0.1, 0, 0.1)));
		scene.unBlock();
		
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