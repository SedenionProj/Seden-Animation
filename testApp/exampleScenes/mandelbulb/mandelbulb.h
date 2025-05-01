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

class MandelbulbPointCloud : public ObjectScene {
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
		wait();
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

class Mandelbrot : public ShaderScene {
public:
	Mandelbrot() : ShaderScene(
		"C:/Users/Gerome/source/repos/Seden-Animation/Seden/assets/shaders/basicPosVS.glsl", 
		"C:/Users/Gerome/source/repos/Seden-Animation/testApp/exampleScenes/mandelbulb/mandelbrotFS.glsl", 15) {
	}
	float zoom = 1.f;//0.249f; 0.135
	float libre = 0.1f;
	float angle = 0.0f;
	vec2 pos = { 0,0 };
	vec3 color = { 0.257,0,0 };
	vec2 uShift = { -0.7524753,-0.5 };

	void animation() override {

		addUniform(ShaderDataType::FLOAT, "zoom", &zoom);
		addUniform(ShaderDataType::FLOAT, "libre", &libre);
		addUniform(ShaderDataType::FLOAT, "angle", &angle);
		addUniform(ShaderDataType::VEC2, "pos", &pos);
		addUniform(ShaderDataType::VEC3, "color", &color);
		addUniform(ShaderDataType::VEC2, "uShift", &uShift, -1, 1, 7);
		
		//animAttach(new VarPositionFun(zoom, [](float t, float dt) -> float { return 0.85*(cos(t*0.1)*0.5f)+0.5f+0.135; }));
		//anim(new ChangeTo(uShift, { -0.7524753, 0.5 }), 6, 0);
		anim(new ChangeTo(zoom, .125f), 5, 0);
		anim(new ChangeTo(pos, vec2( 0,1) ), 2, 0);



		wait();
	}
};

/*
Line::create(a, b);
LineStrip::create({...});
Bezier::create({...});
*/

class Mandelbulb : public ShaderScene {
public:
	Mandelbulb() : ShaderScene(
		"C:/Users/Gerome/source/repos/Seden-Animation/Seden/assets/shaders/basicPosVS.glsl",
		"C:/Users/Gerome/source/repos/Seden-Animation/testApp/exampleScenes/mandelbulb/mandelbulbFS.glsl", 1) {
	}
	float zoom = 0.f;
	float libre = 5.f;
	vec2 pos = { -2,-3.1415 / 2. };
	vec3 color = { .484,1,1 };
	vec3 m_precision = { 0.0001, 100,0};
	vec4 juliaPos = {0.00, 0,0,0};
	float power = 2;
	float uCut = -1.2;

	void animation() override {
		//m_animationSpeed = 10.25;
		addUniform(ShaderDataType::FLOAT, "zoom", &zoom, -1, 1.,7);
		addUniform(ShaderDataType::FLOAT, "libre", &libre, -5, 5);
		addUniform(ShaderDataType::FLOAT, "power", &power, 1, 24);
		addUniform(ShaderDataType::VEC2, "pos", &pos, -4,4);
		addUniform(ShaderDataType::VEC3, "color", &color);
		addUniform(ShaderDataType::VEC3, "m_precision", &m_precision, 0, 0.001, 7);
		addUniform(ShaderDataType::VEC4, "juliaPos", &juliaPos, -1, 1, 3);
		addUniform(ShaderDataType::FLOAT, "uCut", &uCut, -1.5, 1.5, 3);
		//block();
		auto text = Text::create("", {-16 / 9.+0.01,-1+0.01,0}, 7.5, {0.5,0,0.5,0.35});
		
		//unBlock();

		//block();
		//animAttach(new VarPositionFun(pos, [&](float t, float dt) {
		//	text->get<Comp::Text>().setText(
		//		"pos = (" + std::to_string(juliaPos.x) + 
		//		") + (" + std::to_string(juliaPos.y) + 
		//		"i) + (" + std::to_string(juliaPos.z) + 
		//		"j) + (" + std::to_string(juliaPos.w) + 
		//		"k)");
		//	return vec2(t * 0.05 - 2.5, -3.1415 / 2.);
		//	}));
		//unBlock();

		//wait(1);
		//
		//block();
		//anim(new VarPositionFun(juliaPos, [&](float t, float dt) {
		//	return vec4(-sin(t) * cos(t*0.5), sin(t*0.2), 0, 0);
		//	}), 2);
		//unBlock();
		//
		//wait(2);
		//
		//block();
		//anim(new ChangeTo(uCut, .0f), 5.,0, new EaseInOut(5));
		//unBlock();
		//
		//wait(4);
		//
		//block();
		//animAttach(new VarPositionFun(juliaPos, [&](float t, float dt) {
		//	return sin(juliaPos+vec4(-sin(t) * cos(t * 0.5), sin(t * 0.2), 0, 0)*0.1f);
		//	}));
		//unBlock();
		//
		//wait(4);
		//
		//block();
		//anim(new ChangeTo(uCut, -1.2f), 5., 0, new EaseInOut(5));
		//unBlock();
		//
		wait();
	}
};