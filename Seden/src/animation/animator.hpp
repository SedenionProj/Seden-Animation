#pragma once
#include <memory>
#include <glm/glm.hpp>

namespace Seden {
	class Object;
	class Transform;

	class Animator {
	public:
		virtual void update(float time) = 0;
	};

	class MoveTo : public Animator {
	public:
		MoveTo(std::shared_ptr<Object> obj, const glm::vec3& pos);

		void update(float time) override;

	private:
		glm::vec3 m_from;
		const glm::vec3& m_pos;
		Transform& m_transform;
	};
}