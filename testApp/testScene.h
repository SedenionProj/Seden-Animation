#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "src/application.hpp"
#include "src/object/object.hpp"
#include "src/object/components.hpp"
#include "src/animation/curve.hpp"
#include "src/animation/animator.hpp"

using namespace Seden;
using namespace glm;

class TestScene : public ObjectScene {
public:
	void animation() override {
		block();
		auto cam = PerspectiveCamera::create({ 0,0,-2 });
		//cam = OrthographicCamera::create(0, m_width, m_height,0);
		//cam = OrthographicCamera::create(0, 1.1, 1.1,-0.05);
		setCamera(cam);

		auto p = Path::create({
			{ {1,0,0,1}, {-1  ,0.0,0}, 10.f },
			{ {0,0,1,1}, {-0.5,0,0	}, 10.f},
			{ {1,0,1,1}, {-0.3,0.8,0}, 10.f },
			{ {0,1,0,1}, {-0.2,0.8,0}, 10.f }
			});


		auto p3 = Path::create({
			{ {1,0,0,1}, {-1  ,0.0,0},  10.f},
			{ {0,0,1,1}, {-0.5,-0.2,0}, 10.f},
			{ {1,0,1,1}, {-0.3,-0.25,0},10.f},
			{ {1,0,1,1}, { 0.,-0.5,0},	10.f},
			{ {1,0,1,1}, { 0.3,-0.5,0}, 10.f},
			{ {0,1,0,1}, { 0.5,0.8,0},	10.f}
			});
		

		unBlock();

		wait();
	}

};