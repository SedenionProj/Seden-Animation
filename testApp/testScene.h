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
		auto cam = PerspectiveCamera::create({ 0,0,-1 });
		//cam = OrthographicCamera::create(0, m_width, m_height,0);
		//cam = OrthographicCamera::create(0, 1.1, 1.1,-0.05);
		setCamera(cam);

		Comp::GroupObjects lines;
		for (float i = -150; i < 150; i++) {
			lines.addObject(Path::create({
				{ {1,1,1,1}, {-2.1, i/100.f, 0}, 3.f },
				{ {1,1,1,1}, {-2,	i/100.f, 0}, 3.f },
				{ {1,1,1,1}, { 2,	i/100.f, 0}, 3.f },
				{ {1,1,1,1}, { 2.1, i/100.f, 0}, 3.f }  
			}));
		}

		//nimAttach(new Fun( [&](float t, float dt){
		//	p3->get<Comp::Path>().m_vertices[1].position.y = sin(t);
		//));

		unBlock();

		wait();
	}

};