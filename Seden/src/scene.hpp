#pragma once
#include <entt/entt.hpp>
#include <type_traits>
#include <thread>
#include <vector>

#include "src/renderer.hpp"
#include "src/animation/curve.hpp"
#include "src/animation/animation.hpp"
#include "src/logger.h"
#include "src/util/deltaTime.hpp"
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

		template<typename T>
		void animate(VariableAnimationInfo<T> anim);

		void animate(FunctionAnimationInfo anim);

		template<typename Animator, typename... Args>
		void animGroup(const GroupObjects& objects, Args&&... args);

		void animAttach(Animator* anim);

		void block() {
			m_loopSync.block();
			m_FrameBeginSync.waitUntilUnblocked();
		}

		void unBlock() {
			m_loopSync.unBlock();
		}

		entt::registry& getRegistry() { return m_registry; }
		std::mutex m_animationsMutex;

	private:
		void draw();

	private:
		entt::registry m_registry;
		Window& m_window;
		Renderer m_renderer{m_window};
		DeltaTime m_dt;

		Sync m_loopSync;
		Sync m_FrameBeginSync;

		std::vector<std::unique_ptr<Animation>> m_animations;
	};

	template<typename T>
	void Scene::animate(VariableAnimationInfo<T> anim) {
		m_animations.push_back(std::make_unique<VariableAnimation<T>>(anim.var, anim.to, anim.curve, anim.time));
	}

	template<typename Animator, typename... Args>
	void Scene::animGroup(const GroupObjects& objects, Args&&... args)
	{
		float shift = 0;
		for (std::shared_ptr<Object> o : objects) {
			m_animations.push_back(std::make_unique<FunctionAnimation>(new Animator(o,std::forward<Args>(args)...), new EaseInOut(6), 5.f, shift));
			shift += 0.1;
		}
	}
}