#pragma once
#include "src/animation/animator.hpp"
#include "src/util/math.h"

//todo: reconsider using raw ptrs

namespace Seden {
	class Object;

	struct TimedAnimationInfo {
		Animator* anim;
		Curve* curve = new EaseInOut(5);
		float time = 2;
	};

	class Animation {
	public:
		virtual void update(float dt) = 0;
		virtual ~Animation() = default;

		void shiftStart(float sec) {
			t-=sec;
		}

		bool finished = false;

	protected:
		float t = 0;
		float prev_f_t = 0;
	};

	class TimedAnimation : public Animation {
	public:
		TimedAnimation(Animator* anim, Curve* curve, float time, float shift = 0)
			: m_anim(anim), m_curve(curve), m_time(time) {
			shiftStart(shift);
		}

		virtual ~TimedAnimation() {
			delete m_anim;
			delete m_curve;
		}

		void update(float dt) override {
			float step_dt = secondsToStep(dt, m_time);
			t += step_dt;
			if (t < 0) return;
			if (t > 1) finish();
			else at(t);
		}

		void at(float time) {
			float f_t = m_curve->get(time);
			m_anim->update(f_t, f_t - prev_f_t);
			prev_f_t = f_t;
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

    class AttachAnimation : public Animation {
		// todo: detach, fade
    public:
        AttachAnimation(Animator* anim, float shift = 0)
            : m_anim(anim) {
            shiftStart(shift);
        }

        void update(float dt) override {
            t += dt;
            m_anim->update(t, dt);
        }

    private:
        Animator* m_anim;
    };
}