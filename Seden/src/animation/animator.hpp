#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


namespace Seden {
	class Object;
	class Transform;

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
		const glm::vec3& m_pos;
		Transform& m_transform;
	};

	class TranslateBy : public Animator {
	public:
		TranslateBy(std::shared_ptr<Object> obj, const glm::vec3& vec);

		void update(float time, float dt) override;

	private:
		const glm::vec3& m_vec;
		Transform& m_transform;
	};

	class RotateTo : public Animator {
	public:
		RotateTo(std::shared_ptr<Object> obj, const glm::quat& quaterion);

		void update(float time, float dt) override;

	private:
		glm::quat m_from;
		const glm::quat& m_quat;
		Transform& m_transform;
	};

	class ScaleTo : public Animator {
	public:
		ScaleTo(std::shared_ptr<Object> obj, const glm::vec3& scale);

		void update(float time, float dt) override;

	private:
		glm::vec3 m_from;
		const glm::vec3& m_scale;
		Transform& m_transform;
	};
}