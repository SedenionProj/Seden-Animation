#pragma once
#include <entt/entt.hpp>
#include <type_traits>
#include <thread>
#include <vector>

#include "src/renderer.hpp"
#include "src/animation/curve.hpp"
#include "src/animation/animation.hpp"
#include "src/logger.h"
#include "src/deltaTime.hpp"
#include "src/window.hpp"

namespace Seden {
	class Scene {
	public:
		Scene(Window& window) : m_window(window) {}

		void startAnimationLoop();

		void wait(float seconds);

		template<typename T>
		void animate(VariableAnimationInfo<T> anim);

		entt::registry& getRegistry() { return m_registry; }

	private:
		void draw();

	private:
		entt::registry m_registry;
		Window& m_window;
		Renderer m_renderer{m_window};
		DeltaTime m_dt;

		std::vector<std::unique_ptr<Animation>> m_animations;
	};

	template<typename T>
	void Scene::animate(VariableAnimationInfo<T> anim) {
		m_animations.push_back(std::make_unique<VariableAnimation<T>>(anim.var, anim.to, anim.curve, anim.time));
	}
}
