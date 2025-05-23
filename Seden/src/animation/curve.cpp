#include "src/animation/curve.hpp"

namespace Seden {
	float Curve::operator()(float time) const
	{
		return get(time);
	}

	EaseIn::EaseIn(float strength)
		: m_strength(strength) {}

	float EaseIn::get(float time) const
	{
		DEBUG_ASSERT(time >= 0 && time <= 1, "time should be between 0 and 1");

		return glm::pow(time, m_strength);
	}
	
	EaseInOut::EaseInOut(float strength)
		: m_strength(strength) {}

	float EaseInOut::get(float time) const
	{
		DEBUG_ASSERT(time >= 0 && time <= 1, "time should be between 0 and 1");

		if (time < 0.5)
			return 0.5 * glm::pow(2 * time, m_strength);
		else
			return 1.f-0.5 * glm::pow(2 * (1.f - time), m_strength);
	}

}