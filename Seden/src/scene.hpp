#pragma once
#include <entt/entt.hpp>
#include <type_traits>

#include "src/renderer.hpp"
#include "src/animation/curve.hpp"
#include "src/animation/animation.hpp"
#include "src/logger.h"
#include "src/deltaTime.hpp"

namespace Seden {
	class Scene {
	public:
		Scene() {
		}

		void wait(float seconds);
		void animate(Animation& animation);

		template<typename T>
		void animate(T* variable, T to, float seconds, const Curve& curve);

		entt::registry& getRegistry() {
			return m_registry;
		}
	private:
		void draw();
		float secondsToStep(float dt, float sec);

	private:
		entt::registry m_registry;
		Renderer m_renderer;
		DeltaTime dt;
	};
	


	template<typename T>
	inline void Scene::animate(T* variable, T to, float seconds, const Curve& curve)
	{
		// todo: maybe, the function should use concepts
		float step = secondsToStep(dt.getElapsedTime(), seconds);

		T from = *variable;

		for (float time = 0; time < 1.f; time += step) {
			dt.reset();
			float t = curve(time);
			*variable = from * (1.f - t) + to * t;
			m_renderer.beginFrame();
			draw();
			m_renderer.endFrame();
			step = secondsToStep(dt.getElapsedTime(), seconds);
		}
	}

}

