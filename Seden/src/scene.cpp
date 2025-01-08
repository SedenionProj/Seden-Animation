#include "scene.hpp"
#include "object.hpp"

namespace Seden {
	void Scene::wait(uint32_t frames) {
		for (uint32_t i = 0; i < frames; i++) {
			/*
			renderer::draw();
			*/
		}
	}
	void Scene::update() {
		for (auto entity : m_registry.view<PolygonMesh>()) {
			/*
			Renderer::drawPolygonMesh(transform, mesh);
			*/
		}
	}
}
