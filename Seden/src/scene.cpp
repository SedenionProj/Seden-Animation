#include "src/scene.hpp"
#include "src/object/object.hpp"


namespace Seden {
	void Scene::wait(float seconds) {
		std::this_thread::sleep_for(std::chrono::duration<float>(seconds));
	}

    void Scene::animate(FunctionAnimationInfo anim)
    {
        m_animations.push_back(std::make_unique<FunctionAnimation>(anim.anim, anim.curve, anim.time));
    }

	void Scene::startAnimationLoop()
	{
        while (m_window.isRunning()) {


            m_animations.erase(
                std::remove_if(
                    m_animations.begin(), m_animations.end(),
                    [&](const std::unique_ptr<Animation>& anim) {
                        anim->update(m_dt.getElapsedTime());

                        return anim->finished;
                    }
                ),
                m_animations.end()
            );
     

            m_dt.reset();

            m_renderer.beginFrame();
            draw();
            m_renderer.endFrame();
        }
	}

	void Scene::draw() {
		auto t = m_registry.view<PolygonMesh>();
		for (auto entity : m_registry.view<PolygonMesh>()) {
			m_renderer.drawPolygonMesh(m_registry.get<Transform>(entity), m_registry.get<PolygonMesh>(entity));
		}
	}
}
