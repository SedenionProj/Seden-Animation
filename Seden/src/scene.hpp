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
#include "src/window.hpp"
#include "src/util/sync.h"

namespace Seden {
	class Camera;

	class Scene {
	public:
		Scene(Window& window) : m_window(window) {}

		void startAnimationLoop();

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

	private:

		entt::registry m_registry;
		Window& m_window;
		Renderer m_renderer{m_window};

		Sync m_waitSync;
		Sync m_loopSync;		 
		bool m_waiting = false;
		float m_waitTime = 0;

		std::vector<std::unique_ptr<Animation>> m_animations;

		friend class Object;
	};
	extern Scene* s_scene;
}