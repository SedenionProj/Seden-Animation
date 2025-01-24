#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


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
}