#include "src/scene.hpp"
#include "src/object/object.hpp"


namespace Seden {
	void Scene::wait(float seconds) {
		Clock clock;
		for (float i = 0.f; i < seconds; i += clock.getElapsedTimeAndReset()) {
			std::this_thread::sleep_for(std::chrono::microseconds(10));
			if(!m_window.isRunning()) break;
		}
	}

	void Scene::setCamera(std::shared_ptr<PerspectiveCamera> camera)
	{
		m_renderer.setCamera(camera);
	}

	void Scene::anim(Animator* anim, float time, float shift, Curve* curve)
	{
		m_animations.push_back(std::make_unique<TimedAnimation>(anim, curve, time, shift));
	}

	void Scene::animAttach(Animator* anim, float shift)
	{
		m_animations.push_back(std::make_unique<AttachAnimation>(anim, shift));
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
			m_renderer.endFrame();

			m_FrameBeginSync.unBlock();
			
		}
		DEBUG_MSG("end of animation loop");
	}


	void Scene::draw() {
		m_renderer.drawDebugGui();

		m_registry.view<Comp::Transform, Comp::PolygonMesh>().each([this](const auto object, auto& transform, auto& mesh) {
			if (Comp::Parent* comp = m_registry.try_get<Comp::Parent>(object); comp && comp->m_parent) { // analyze ";"
				//todo, use group
				//glm::mat4 result = comp->m_parent->getRecurse<Comp::Transform>(
				//	[](Comp::Transform& a, Comp::Transform& b) -> glm::mat4 {
				//		return a.getTransform() * b.getTransform();
				//	}
				//);
				//m_renderer.drawConvexPolygon(transform, mesh);
			}
			else {
				m_renderer.drawConvexPolygon(transform, mesh);
			}
		});

		m_registry.view<Comp::Transform, Comp::SimpleText>().each([this](const auto object, auto& transform, auto& text) {
			m_renderer.drawSimpleText(transform, text);
		});

		m_registry.view<Comp::Transform, Comp::GroupObjects, Comp::Text>().each([this](const auto object, auto& transform, auto& letters, auto& text) {
			m_renderer.drawText(transform, letters, text);
		});
	}


}
