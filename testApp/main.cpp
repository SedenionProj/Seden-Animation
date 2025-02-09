#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "src/application.hpp"
#include "src/object/object.hpp"
#include "src/object/components.hpp"
#include "src/animation/curve.hpp"

/*todo: 
	- quad, point, custom shader, spline objects...
	- rethink scene system, entry point, cli, shared_ptr and new
	- sort blending
*/ 

using namespace Seden;
using namespace glm;

class TestApp : public Seden::Application {
	std::shared_ptr<OrthographicCamera> cam;
public:
	void animation() override {
		cam = OrthographicCamera::create(0,1280,0,720);
		scene.setCamera(cam);
		scene3();
		//cam->get<Comp::Transform>().setPosition({ 0,0,0 });

		DEBUG_MSG("fin");
	}
	void scene3() {
		scene.block();
		Comp::GroupObjects squares;
		for (int y = 0; y < 720/4; y++) {
			for (int x = 0; x < 1280/4; x++) {
				squares.addObject(Point::create(4));
				auto sq = squares.getObject(squares.size()-1);
				sq->get<Comp::Transform>().setPosition({x*4,y*4,0});
				sq->get<Comp::Color>().m_color = vec4(vec3(myHash(x+y*1280)),1);
			}
		}

		scene.unBlock();
	}

	void scene2() {
		Comp::GroupObjects texts;

		scene.block();
		auto seed = texts.addObject(Text::create("seed = 42   (graine)", {-2,-3,3}, 1));
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
			auto t = texts.addObject(Text::create("rand() = " + std::to_string(rand()), vec3(-2, y-1, 3), 1));
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
		auto seed2 = texts.addObject(Text::create("seed = 587   (graine)", { 7,-3,3 }, 1));
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
			auto t = texts.addObject(Text::create("rand() = " + std::to_string(rand()), vec3(7, y - 1, 3), 1));
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
		scene.m_animationSpeed =1.f;
		Comp::GroupObjects texts;
		Comp::GroupObjects squares;

		scene.block();

		for (int y = 0; y < 170; y++) {
			auto t = texts.addObject(Text::create("n = " + std::to_string(y) + " : hash(" + std::to_string(y) + ") = " + std::to_string(myHash(y)), vec3(-1, y/2.f, 0.0), 0.2f));
			
			for (int i = 0; i < t->get<Comp::Text>().getLength(); i++) {
				auto letter = t->get<Comp::GroupObjects>().getObject(i);
				//scene.animAttach(new Wave(letter, 0.1), (y + i) / 10.f);
				letter->get<Comp::Color>().m_color = { 0.7,0.2,0.6,0. };
				scene.anim(new ChangeTo(letter->get<Comp::Color>().m_color, glm::vec4(0.7, 0.2, 0.6, 1.)), 2.5f, (pow(y, 0.5f) * 10 + i) / 100.f);
			}
		}
		scene.anim(new TranslateBy(cam, { 1.5,60,-3 }), 10);
		scene.unBlock();
		scene.wait(10);

		scene.block();
		for (auto t : texts) {
			uint32_t l = t->get<Comp::Text>().getLength()-8;
			for (int i = 0; i < l; i++) {
				auto letter = t->get<Comp::GroupObjects>().getObject(i);
				scene.anim(new ChangeTo(letter->get<Comp::Color>().m_color, glm::vec4(0.7, 0.2, 0.6, 0.)), 1.f, i / 10.f);
			}
		}
		int count = 0;
		int nbX = 17;
		int nbY = 10;
		for (auto t : texts) {
			
			float scX = 3;
			int x = (count % nbX);
			int y = (count / nbX);

			if (count < 10)
				scene.anim(new MoveTo(t, vec3(-1.6 + 0.5 + (-nbX / 2.f + x) * 1.05, 0.5 + (-nbY / 2.f + y) * 1.05, 0.01)), 5);
			else if(count < 6*17-4)
				scene.anim(new MoveTo(t, vec3(-1.7 + 0.5 + (-nbX / 2.f + x) * 1.05, 0.5 + (-nbY / 2.f + y) * 1.05, 0.01)), 5);
			else
				scene.anim(new MoveTo(t, vec3(-1.8 + 0.5 + (-nbX / 2.f + x) * 1.05, 0.5 + (-nbY / 2.f + y) * 1.05, 0.01)), 5);

			squares.addObject(Quad::create());
			auto s = squares.getObject(count);
			s->get<Comp::Transform>().setPosition(vec3(0.5+(-nbX /2.f+x)*1.05, 0.5 + (-nbY / 2.f + y)*1.05, 0.01));
			auto& textestr = t->get<Comp::Text>().getText();
			std::string textnum = textestr.substr(textestr.size() - 8);
			float num = std::stof(textnum);
			
			auto& v =s->get<Comp::PolygonMesh>().getVertices();
			scene.anim(new ChangeTo(v[0].color, vec3(num)), 2, 2+(10+ count)/100.f);
			scene.anim(new ChangeTo(v[1].color, vec3(num)), 2, 2+(10+ count)/100.f);
			scene.anim(new ChangeTo(v[2].color, vec3(num)), 2, 2+(10+ count)/100.f);
			scene.anim(new ChangeTo(v[3].color, vec3(num)), 2, 2+(10+ count)/100.f);
			count++;
		}
		scene.anim(new MoveTo(cam, { 0,0,-5 }), 5);
		scene.unBlock();
		scene.wait(10);

		scene.block();
		for (int i = 170; i < 10000; i++) {
			int x = (i % nbX);
			int y = (i / nbX);

			squares.addObject(Quad::create());
			auto s = squares.getObject(i);
			s->get<Comp::Transform>().setPosition(vec3(0.5 + (-nbX / 2.f + x) * 1.05, 0.5 + (-nbY / 2.f + y) * 1.05, 0.01));
			auto& v = s->get<Comp::PolygonMesh>().getVertices();
			float num = myHash(i);
			scene.anim(new ChangeTo(v[0].color, vec3(num)), 2);
			scene.anim(new ChangeTo(v[1].color, vec3(num)), 2);
			scene.anim(new ChangeTo(v[2].color, vec3(num)), 2);
			scene.anim(new ChangeTo(v[3].color, vec3(num)), 2);
		}
		scene.unBlock();

		scene.anim(new MoveTo(cam, { 0,+333,-5 }), 20);


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