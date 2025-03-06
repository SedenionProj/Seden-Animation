#include "src/scene.hpp"
#include "src/object/object.hpp"

namespace Seden {
	void Scene::wait(float seconds) {
		m_waiting = true;
		while (m_waitTime < seconds) {
			if(!m_window->isRunning()) break;
		}
		m_waiting = false;
		m_waitTime = 0;
	}

	void Scene::setCamera(std::shared_ptr<Camera> camera)
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
		while (m_window->isRunning()) {
			m_waitSync.waitUntilUnblocked();
			m_loopSync.block();

			bool hasAnimToRemove = false;
			float dt = m_window->isRecording() ? 1.f/m_window->getFrameRate() : clock.getElapsedTimeAndReset();
			dt *= m_animationSpeed;
			if (m_waiting) {
				m_waitTime += dt;
			}

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

			m_loopSync.unBlock();
			
		}
		DEBUG_MSG("end of animation loop");
	}


	void Scene::draw() {
		m_renderer.drawDebugGui();

		m_registry.view<Comp::Transform, Comp::PolygonMesh>().each([this](const auto object, auto& transform, auto& mesh) {
			if (Comp::Parent* comp = m_registry.try_get<Comp::Parent>(object); comp && comp->m_parent) {
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

		m_registry.view<Comp::Transform, Comp::GroupObjects, Comp::Text>().each([this](const auto object, auto& transform, auto& letters, auto& text) {
			m_renderer.drawText(transform, letters, text);
		});

		m_registry.view<Comp::Transform, Comp::Color, Comp::Point>().each([this](const auto object, auto& transform, auto& color, auto& point) {
			m_renderer.drawPoint(transform, color, point);
		});

		m_registry.view<Comp::Transform, Comp::Shader>().each([this](const auto object, auto& transform, auto& shader) {
			m_renderer.drawShaderQuad(transform, shader);
		});
	}


}
