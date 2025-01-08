#pragma once
#include <entt/entt.hpp>
#include "src/renderer.hpp"

namespace Seden {
	class Scene {
	public:
		Scene() {
			Renderer::init();
		}

		void wait(uint32_t frames);


		entt::registry& getRegistry() {
			return m_registry;
		}
	private:
		void update();

	private:
		entt::registry m_registry;
	};
}