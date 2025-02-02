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
	class PerspectiveCamera;

	class Scene {
	public:
		Scene(Window& window) : m_window(window) {}

		void startAnimationLoop();

		void setCamera(std::shared_ptr<PerspectiveCamera> camera);

		void wait(float seconds);

		void block() {
			m_loopSync.block();
			m_FrameBeginSync.waitUntilUnblocked();
		}
		void unBlock() {
			m_loopSync.unBlock();
		}

		void anim(Animator* anim, float time = 1, float shift = 0, Curve* curve = new EaseInOut(5));
		void animAttach(Animator* anim, float shift = 0);

	private:
		void draw();

	private:
		entt::registry m_registry;
		Window& m_window;
		Renderer m_renderer{m_window};

		Sync m_loopSync;
		Sync m_FrameBeginSync;

		std::vector<std::unique_ptr<Animation>> m_animations;

		friend class Object;
	};
	extern Scene* s_scene;
}