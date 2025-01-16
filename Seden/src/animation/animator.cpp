#include "src/animation/animator.hpp"
#include "src/logger.h"
#include "src/object/object.hpp"
#include "src/util/math.h"

namespace Seden {
	MoveTo::MoveTo(std::shared_ptr<Object> obj, const glm::vec3& pos)
		: m_pos(pos), m_transform(obj->get<Transform>()) {
		m_from = m_transform.getPosition();
	}

	void MoveTo::update(float time)
	{
		m_transform.setPosition(lerp(m_from, m_pos, time));
	}
}