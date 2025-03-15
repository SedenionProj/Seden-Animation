#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "src/application.hpp"
#include "src/object/object.hpp"
#include "src/object/components.hpp"
#include "src/animation/curve.hpp"
#include "src/animation/animator.hpp"

using namespace Seden;
using namespace glm;

struct Triplex {
	float x, y, z;
	void add(const Triplex& a) {
		x += a.x;
		y += a.y;
		z += a.z;
	}

	void pow(float p) {
		float r = sqrt(x * x + y * y + z * z);
		float theta = acos(z / r);
		float phi = atan2(y, x);
		r = std::pow(r, p);
		theta *= p;
		phi *= p;
		x = r * sin(theta) * cos(phi);
		y = r * sin(theta) * sin(phi);
		z = r * cos(theta);
	}
};

class TestScene : public ObjectScene {
	//std::shared_ptr<OrthographicCamera> cam;
	std::shared_ptr<PerspectiveCamera> cam;

public:
	void animation() override {
		cam = PerspectiveCamera::create({ 0,0,-2 });
		//cam = OrthographicCamera::create(0, m_width, m_height,0);
		//cam = OrthographicCamera::create(0, 1.1, 1.1,-0.05);
		setCamera(cam);
		scene1();
		
		DEBUG_MSG("fin");
	}
	void scene1() {
		m_animationSpeed = 1.f;
		Comp::GroupObjects squares;

		block();
		//animAttach(new RotateTo(cam, { 1,0,1,0 }));

		animAttach(new PositionFun(cam, [](float t, float dt) { return 3.f * vec3(sin(t), 0, cos(t)); }));
		float step = 0.05;
		for (float y = -1.0; y < 1.0; y += step) {
			for (float x = -1.0; x < 1.0; x += step) {
				bool edge = false;
				for (float z = -1.0; z < 1.0; z += step) {
					if (mandelbulb(x, y, z)) {
						if (edge == false) {
							edge = true;
							squares.addObject(Point::create(5, { x,y,z }));
						}
					}
					else {
						if (edge == true) {
							edge = false;
						}
					}
				}
			}
		}

		unBlock();

	}

	

private:
	bool mandelbrot(float x, float y) {
		float xn = 0, yn = 0, xn_temp;
		for (int i = 0; i < 10; i++) {
			xn_temp = xn * xn - yn * yn + x;
			yn = 2 * xn * yn + y;
			xn = xn_temp;

			if ((xn * xn + yn * yn) > 16)
				return false;
		}
		return true;
	}

	bool mandelbulb(float x, float y, float z) {
		Triplex c = { x, y, z };

		for (int i = 0; i < 1000; i++) {
			c.pow(8);
			c.add({ x, y, z });
			if (c.x * c.x + c.y * c.y + c.z * c.z > 100)
				return false;
		}
		return true;
	}
};

class TestShader : public ShaderScene {
public:
	TestShader() : ShaderScene(
		"C:/Users/Gerome/source/repos/Seden-Animation/testApp/exampleScenes/mandelbulb/mandelbrotVS.glsl", 
		"C:/Users/Gerome/source/repos/Seden-Animation/testApp/exampleScenes/mandelbulb/mandelbrotFS.glsl") {}
	float val = 0.0f;
	void animation() override {
		
		addUniform(ShaderDataType::FLOAT, "test", &val);

		anim(new ChangeTo(val, 1.f), 5);

		wait();
	}
};