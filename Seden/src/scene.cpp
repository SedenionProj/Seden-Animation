#include "src/scene.hpp"
#include "src/object/object.hpp"


namespace Seden {
	void Scene::wait(float seconds) {
		Clock clock;
		for (float i = 0.f; i < seconds; i += clock.getElapsedTimeAndReset()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			if(!m_window.isRunning()) break;
		}
	}

    void Scene::animate(FunctionAnimationInfo anim)
    {
		std::lock_guard<std::mutex> lock(m_animationsMutex);
        m_animations.push_back(std::make_unique<FunctionAnimation>(anim.anim, anim.curve, anim.time));
    }

    void Scene::animAttach(Animator* anim)
    {
        m_animations.push_back(std::make_unique<AttachAnimation>(anim));
    }

	void Scene::startAnimationLoop()
	{
		Clock clock;

		while (m_window.isRunning()) {
			m_loopSync.waitUntilUnblocked();
			m_FrameBeginSync.block();

			bool hasAnimToRemove = false;
			float dt = clock.getElapsedTimeAndReset();
			for (auto& anim : m_animations) {
				anim->update(dt);
				if (anim->finished)
					hasAnimToRemove = true;
			}
			if (hasAnimToRemove) {
				m_animations.erase(
					std::remove_if(
						m_animations.begin(), m_animations.end(),
						[&](const std::unique_ptr<Animation>& anim) {
							return anim->finished;
						}),
					m_animations.end());
			}	

			m_renderer.m_stats.frameTimeElapsed = dt;
			m_renderer.m_stats.animationCount = m_animations.size();
			m_renderer.m_stats.objectAliveCount = m_registry.storage<entt::entity>().size();


			m_renderer.beginFrame();
			draw();
			m_renderer.drawDebugGui();
			m_renderer.endFrame();

			m_FrameBeginSync.unBlock();
			
		}
		DEBUG_MSG("end of animation loop");
	}

	
    void Scene::setCamera(std::shared_ptr<PerspectiveCamera> camera)
    {
        m_renderer.setCamera(camera);
    }

	void Scene::draw() {
		m_registry.view<Comp::Transform, Comp::PolygonMesh>().each([this](auto& transform, auto& mesh) {
			m_renderer.drawConvexPolygon(transform, mesh);
		});


	}


}
