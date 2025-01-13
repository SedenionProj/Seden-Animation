#include "src/scene.hpp"
#include "src/object.hpp"


namespace Seden {
	void Scene::wait(float seconds) {
		float step = secondsToStep(dt.getElapsedTime(), seconds);
		for (float t = 0; t < 1.f; t +=step ) {
			dt.reset();
			m_renderer.beginFrame();
			draw();
			m_renderer.endFrame();
			step = secondsToStep(dt.getElapsedTime(), seconds);
		}
	}
	void Scene::animate(Animation& animation) {
		float step = 0.01f;
		EaseInOut curve(10.f);

		for (float time = 0; time < 1.f; time += step) {
			float t = curve(time);
			animation.update(t);
		}

	}

	void Scene::draw() {
		auto t = m_registry.view<PolygonMesh>();
		for (auto entity : m_registry.view<PolygonMesh>()) {
			m_renderer.drawPolygonMesh(m_registry.get<Transform>(entity), m_registry.get<PolygonMesh>(entity));
		}
	}
	float Scene::secondsToStep(float dt, float sec)
	{
		return dt/sec;
	}
}
