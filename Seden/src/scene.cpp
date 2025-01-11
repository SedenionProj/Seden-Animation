#include "scene.hpp"
#include "object.hpp"

namespace Seden {
	void Scene::wait(uint32_t frames) {
		for (uint32_t i = 0; i < frames; i++) {
			m_renderer.beginFrame();
			update();
			m_renderer.endFrame();
			
		}
	}
	void Scene::update() {
		auto t = m_registry.view<PolygonMesh>();
		for (auto entity : m_registry.view<PolygonMesh>()) {
			m_renderer.drawPolygonMesh(m_registry.get<Transform>(entity), m_registry.get<PolygonMesh>(entity));
		}
	}
}
