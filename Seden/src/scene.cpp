#include "src/scene.hpp"
#include "src/object/object.hpp"


namespace Seden {
	void Scene::wait(float seconds) {
		std::this_thread::sleep_for(std::chrono::duration<float>(seconds));
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
		while (m_window.isRunning()) {
			m_loopSync.waitUntilUnblocked();
			m_FrameBeginSync.block();

			bool hasAnimToRemove = false;
			for (auto& anim : m_animations) {
				anim->update(m_dt.getElapsedTime());
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

			std::cout << 1.f / m_dt.getElapsedTime() << "\n";
			m_dt.reset();

			m_renderer.beginFrame();
			draw();
			m_renderer.endFrame();

			m_FrameBeginSync.unBlock();
		}
	}


    void Scene::setCamera(std::shared_ptr<PerspectiveCamera> camera)
    {
        m_renderer.setCamera(camera);
    }

	void Scene::draw() {
		for (auto entity : m_registry.view<PolygonMesh>()) {
			m_renderer.drawPolygonMesh(
				m_registry.get<Transform>(entity), 
				m_registry.get<PolygonMesh>(entity));
		}
	}	
}
