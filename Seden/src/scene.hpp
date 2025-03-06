#pragma once
#include <entt/entt.hpp>
#include <type_traits>
#include <thread>
#include <vector>

#include "src/renderer.hpp"
#include "src/animation/curve.hpp"
#include "src/animation/animation.hpp"
#include "src/logger.h"
#include "src/util/clock.hpp"
#include "src/util/sync.h"
#include "src/window.hpp"

namespace Seden {
	class Camera;
	class window;

	class Scene {
	public:
		Scene() = default;

		virtual void animation() = 0;

		void setCamera(std::shared_ptr<Camera> camera);

		void wait(float seconds);

		void block() {
			m_waitSync.block();
			m_loopSync.waitUntilUnblocked();
		}
		void unBlock() {
			m_waitSync.unBlock();
		}

		void anim(Animator* anim, float time = 1, float shift = 0, Curve* curve = new EaseInOut(5));
		void animAttach(Animator* anim, float shift = 0);

		float m_animationSpeed = 1;

	private:
		void draw();
		void startAnimationLoop();
		void init(Window* window) {
			m_window = window;
			m_renderer.init(window);
		}

	private:
		entt::registry m_registry;
		Window* m_window;
		Renderer m_renderer;

		Sync m_waitSync;
		Sync m_loopSync;		 
		bool m_waiting = false;
		float m_waitTime = 0;

		std::vector<std::unique_ptr<Animation>> m_animations;

		friend class Object;
		friend class Application;
	};
	extern Scene* s_scene;
}