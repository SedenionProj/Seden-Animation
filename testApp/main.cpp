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
		
		DEBUG_MSG("test");

		auto o = Quad::create(scene);
		o->get<PolygonMesh>().setColor({ 0.7,0.5,0.7 });

		

		//scene.animate(&o->get<Transform>().getTransform(),
		//	glm::translate(o->get<Transform>().getTransform(), glm::vec3(1.,1.,0)),
		//	1,EaseInOut(6));
		//
		/*
		scene.animate(&var1, var2,						  duration, curve);
		scene.animate(obj,			 Animation(param...), duration, curve);
		scene.animate(Group(obj...), Animation(param...), duration, curve);

		animate(Animation a){
			renderer::send(a);
		}

		renderer::loop(){
			for(anim : animationList){
				anim.update(dt);
			}

			renderer::draw();
		}
		*/


	}

private:
	
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