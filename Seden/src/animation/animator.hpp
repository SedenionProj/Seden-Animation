#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "src/util/math.h"

namespace Seden {
	class Object;

	namespace Comp {
		class Transform;
	}

	class Animator {
	public:
		virtual void update(float time, float dt) = 0;
	};

	class MoveTo : public Animator {
	public:
		MoveTo(std::shared_ptr<Object> obj, const glm::vec3& pos);

		void update(float time, float dt) override;

	private:
		glm::vec3 m_from;
		glm::vec3 m_pos;
		Comp::Transform& m_transform;
	};

	class TranslateBy : public Animator {
	public:
		TranslateBy(std::shared_ptr<Object> obj, const glm::vec3& vec);

		void update(float time, float dt) override;

	private:
		glm::vec3 m_vec;
		Comp::Transform& m_transform;
	};

	class RotateTo : public Animator {
	public:
		RotateTo(std::shared_ptr<Object> obj, const glm::quat& quaterion);

		void update(float time, float dt) override;

	private:
		glm::quat m_from;
		glm::quat m_quat;
		Comp::Transform& m_transform;
	};

	class ScaleTo : public Animator {
	public:
		ScaleTo(std::shared_ptr<Object> obj, const glm::vec3& scale);

		void update(float time, float dt) override;

	private:
		glm::vec3 m_from;
		glm::vec3 m_scale;
		Comp::Transform& m_transform;
	};

	class Wave : public Animator {
	public:
		Wave(std::shared_ptr<Object> obj, float amplitude = 1, float frequency = 1, glm::vec3 direction = { 0,1,0 });

		void update(float time, float dt) override;

		float m_amplitude;
		float m_frequency;
		glm::vec3 m_direction;
		Comp::Transform& m_transform;
	};
	
	template<typename T>
	class ChangeTo : public Animator {
	public:
		ChangeTo(T& var, T to)
			: m_var(var), m_to(to) {
			m_from = var;
		}

		void update(float time, float dt) override {
			m_var = Seden::lerp(m_from, m_to, time);
		}

	private:
		T m_from;
		T m_to;
		T& m_var;
	};
}