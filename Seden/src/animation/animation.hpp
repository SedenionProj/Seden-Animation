#pragma once
#include "src/animation/animator.hpp"
#include "src/util/math.h"

namespace Seden {
	template<typename T>
	struct VariableAnimationInfo {
		T* var;
		T to;
		const Curve& curve = EaseInOut(5);
		float time = 2;
	};

	class Animation {
	public:
		virtual void update(float dt) = 0;
		bool finished = false;

	protected:
		float t = 0;
	};

	template<typename T>
	class VariableAnimation : public Animation {
	public:
		VariableAnimation(T* var, T to, const Curve& curve, float time) 
			: m_var(var), m_from(*var), m_to(to), m_curve(curve), m_time(time) {}

		void update(float dt) override {
			t += secondsToStep(dt, m_time);
			if (t > 1) finish();
			else at(t);
		}

	private:
		void at(float time) {
			float f_t = m_curve(time);
			*m_var = (1.f - f_t) * m_from + m_to * f_t;
		}

		void finish() {
			finished = true;
			at(1.f);
		}

		T* m_var;
		T m_from;
		T m_to;
		const Curve& m_curve = EaseInOut(5);
		float m_time = 2;
	};
}