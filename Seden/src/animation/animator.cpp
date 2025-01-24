#include "src/animation/animator.hpp"
#include "src/logger.h"
#include "src/object/object.hpp"
#include "src/util/math.h"

namespace Seden {
	MoveTo::MoveTo(std::shared_ptr<Object> obj, const glm::vec3& pos)
		: m_pos(pos), m_transform(obj->get<Comp::Transform>()) {
		m_from = m_transform.getPosition();
	}

	void MoveTo::update(float time, float dt)
	{
		m_transform.setPosition(lerp(m_from, m_pos, time));
	}

	TranslateBy::TranslateBy(std::shared_ptr<Object> obj, const glm::vec3& vec)
		: m_vec(vec), m_transform(obj->get<Comp::Transform>()) {}

	void TranslateBy::update(float time, float dt)
	{
		m_transform.translate(m_vec * dt);
	}

	
	RotateTo::RotateTo(std::shared_ptr<Object> obj, const glm::quat& quaterion)
		: m_quat(quaterion), m_transform(obj->get<Comp::Transform>()){
		m_from = m_transform.getRotation();
	}

	void RotateTo::update(float time, float dt)
	{
		m_transform.setRotation(glm::lerp(m_from, m_quat, time));
	}

	ScaleTo::ScaleTo(std::shared_ptr<Object> obj, const glm::vec3& scale)
		: m_scale(scale), m_transform(obj->get<Comp::Transform>()){
		m_from = m_transform.getScale();

	}

	void ScaleTo::update(float time, float dt)
	{
		m_transform.setScale(lerp(m_from, m_scale, time));
	}

}