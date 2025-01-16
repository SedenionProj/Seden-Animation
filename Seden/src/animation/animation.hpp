#pragma once
#include "src/animation/animator.hpp"
#include "src/util/math.h"

//todo: reconsider using raw ptrs

namespace Seden {
	class Object;

	template<typename T>
	struct VariableAnimationInfo {
		T* var;
		T to;
		Curve* curve = new EaseInOut(5);
		float time = 2.f;
	};

	struct FunctionAnimationInfo {
		Animator* anim;
		Curve* curve = new EaseInOut(5);
		float time = 2;
	};

	class Animation {
	public:
		virtual void update(float dt) = 0;
		virtual ~Animation() = default;

		void shiftStart(float sec) {
			t = -sec;
		}

		bool finished = false;

	protected:
		float t = 0;
	};

	template<typename T>
	class VariableAnimation : public Animation {
	public:
		VariableAnimation(T* var, T to, Curve* curve, float time, float shift = 0) 
			: m_var(var), m_from(*var), m_to(to), m_curve(curve), m_time(time) {
			shiftStart(shift);
		}

		void update(float dt) override {
			t += secondsToStep(dt, m_time);
			if (t < 0) return;
			if (t > 1) finish();
			else at(t);
		}

	private:
		void at(float time) {
			float f_t = m_curve->get(time);
			*m_var = (1.f - f_t) * m_from + m_to * f_t;
		}

		void finish() {
			finished = true;
			at(1.f);
		}

		T* m_var;
		T m_from;
		T m_to;
		Curve* m_curve;
		float m_time = 2;
	};

	class FunctionAnimation : public Animation {
	public:
		FunctionAnimation(Animator* anim, Curve* curve, float time, float shift = 0)
			:  m_anim(anim), m_curve(curve), m_time(time) {
			shiftStart(shift);
		}

		virtual ~FunctionAnimation(){
			delete m_anim;
		}

		void update(float dt) override {
			t += secondsToStep(dt, m_time);
			if (t < 0) return;
			if (t > 1) finish();
			else at(t);
		}

		void at(float time) {
			float f_t = m_curve->get(time);
			m_anim->update(f_t);
		}

		// todo: pause/resume/stop

	private:
		void finish() {
			finished = true;
			at(1.f);
		}

	private:
		Animator* m_anim;
		Curve* m_curve;
		float m_time;
	};
}