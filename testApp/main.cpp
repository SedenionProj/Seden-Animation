#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "src/application.hpp"
#include "src/object/object.hpp"
#include "src/object/components.hpp"
#include "src/animation/curve.hpp"

/*todo: 
	- quad, point, custom shader, spline objects...
	- rethink scene system, entry point, cli, shared_ptr and new
*/ 

using namespace Seden;
using namespace glm;

class TestApp : public Seden::Application {
	std::shared_ptr<PerspectiveCamera> cam;
public:
	void animation() override {
		cam = PerspectiveCamera::create();
		scene.setCamera(cam);
		scene2();

		DEBUG_MSG("fin");
	}

	void scene2() {
		Comp::GroupObjects texts;

		scene.block();
		auto seed = texts.addObject(Text::create("seed = 42   (graine)", {-2,-3,3}));
		for (int i = 0; i < seed->get<Comp::Text>().getLength(); i++) {
			auto letter = seed->get<Comp::GroupObjects>().getObject(i);
			letter->get<Comp::Color>().m_color = { 0.7,0.2,0.6,0. };
			scene.animAttach(new Wave(letter, 0.01), i/3.);
			scene.anim(new ChangeTo(letter->get<Comp::Color>().m_color, glm::vec4(0.7, 0.2, 0.3, 1.)), 1.f, i / 30.f);
		}
		scene.unBlock();

		scene.wait(1);

		scene.block();
		for (int y = 0; y < 100; y++) {
			auto t = texts.addObject(Text::create("rand() = " + std::to_string(rand()), vec3(-2, y-1, 3)));
			for (int i = 0; i < t->get<Comp::Text>().getLength(); i++) {
				auto letter = t->get<Comp::GroupObjects>().getObject(i);
				letter->get<Comp::Color>().m_color = { 0.7,0.2,0.6,0. };
				scene.anim(new ChangeTo(letter->get<Comp::Color>().m_color, glm::vec4(0.7, 0.2, 0.6, 1.)), 2.5f, (pow(y, 0.75f) * 10 + i) / 100.f);
			}
		}
		scene.unBlock();

		scene.wait(2);

		scene.anim(new TranslateBy(cam, { 1.5,95,0 }), 10);
		scene.wait(10);
		scene.anim(new TranslateBy(cam, { 4,-95,-3 }), 2);
		scene.wait(2);

		scene.block();
		auto seed2 = texts.addObject(Text::create("seed = 587   (graine)", { 7,-3,3 }));
		for (int i = 0; i < seed2->get<Comp::Text>().getLength(); i++) {
			auto letter = seed2->get<Comp::GroupObjects>().getObject(i);
			letter->get<Comp::Color>().m_color = { 0.7,0.2,0.6,0. };
			scene.animAttach(new Wave(letter, 0.01), i / 3.);
			scene.anim(new ChangeTo(letter->get<Comp::Color>().m_color, glm::vec4(0.7, 0.2, 0.3, 1.)), 1.f, i / 30.f);
		}
		scene.unBlock();

		scene.wait(1);

		scene.block();
		for (int y = 0; y < 100; y++) {
			auto t = texts.addObject(Text::create("rand() = " + std::to_string(rand()), vec3(7, y - 1, 3)));
			for (int i = 0; i < t->get<Comp::Text>().getLength(); i++) {
				auto letter = t->get<Comp::GroupObjects>().getObject(i);
				letter->get<Comp::Color>().m_color = { 0.7,0.2,0.6,0. };
				scene.anim(new ChangeTo(letter->get<Comp::Color>().m_color, glm::vec4(0.7, 0.2, 0.6, 1.)), 2.5f, (pow(y, 0.75f) * 10 + i) / 100.f);
			}
		}
		scene.unBlock();

		scene.wait(2);

		scene.anim(new TranslateBy(cam, { 0,95,0 }), 10);
	}

	void scene1() {
		Comp::GroupObjects texts;
		scene.block();

		for (int y = 0; y < 100; y++) {
			auto t = texts.addObject(Text::create("n = " + std::to_string(y) + " : hash(" + std::to_string(y) + ") = " + std::to_string(myHash(y)), vec3(0, y, 3)));
			for (int i = 0; i < t->get<Comp::Text>().getLength(); i++) {
				auto letter = t->get<Comp::GroupObjects>().getObject(i);
				scene.animAttach(new Wave(letter, 0.1), (y + i) / 10.f);
				letter->get<Comp::Color>().m_color = { 0.7,0.2,0.6,0. };
				scene.anim(new ChangeTo(letter->get<Comp::Color>().m_color, glm::vec4(0.7, 0.2, 0.6, 1.)), 0.5f, (y * 10 + i) / 30.f);
			}

		}
		scene.anim(new TranslateBy(cam, { 1.5,100,-5 }), 10);

		scene.unBlock();
	}

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