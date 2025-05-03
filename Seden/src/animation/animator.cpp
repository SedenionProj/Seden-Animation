#include "src/animation/animator.hpp"
#include "src/object/object.hpp"


namespace Seden {
	// MoveTo
	MoveTo::MoveTo(std::shared_ptr<Object> obj, const glm::vec3& pos)
		: m_pos(pos), m_transform(obj->get<Comp::Transform>()) {
		m_from = m_transform.getPosition();
	}

	void MoveTo::update(float time, float dt)
	{
		m_transform.setPosition(lerp(m_from, m_pos, time));
	}

	// TranslateBy
	TranslateBy::TranslateBy(std::shared_ptr<Object> obj, const glm::vec3& vec)
		: m_vec(vec), m_transform(obj->get<Comp::Transform>()) {}

	void TranslateBy::update(float time, float dt)
	{
		m_transform.translate(m_vec * dt);
	}

	// RotateTo
	RotateTo::RotateTo(std::shared_ptr<Object> obj, const glm::quat& quaterion)
		: m_quat(quaterion), m_transform(obj->get<Comp::Transform>()){
		m_from = m_transform.getRotation();
	}

	void RotateTo::update(float time, float dt)
	{
		m_transform.setRotation(glm::lerp(m_from, m_quat, time));
	}

	// ScaleTo
	ScaleTo::ScaleTo(std::shared_ptr<Object> obj, const glm::vec3& scale)
		: m_scale(scale), m_transform(obj->get<Comp::Transform>()){
		m_from = m_transform.getScale();

	}

	void ScaleTo::update(float time, float dt)
	{
		m_transform.setScale(lerp(m_from, m_scale, time));
	}

	// Wave
	Wave::Wave(std::shared_ptr<Object> obj, float amplitude, float frequency, glm::vec3 direction)
		: m_transform(obj->get<Comp::Transform>()), m_amplitude(amplitude), m_frequency(frequency), m_direction(direction){}

	void Wave::update(float time, float dt) {
		float wave = m_amplitude * cos(m_frequency*time*PI)* m_frequency*PI;
		m_transform.translate(m_direction * wave *dt);
	}

	// PositionFun

	PositionFun::PositionFun(std::shared_ptr<Object> obj, std::function<glm::vec3(float, float)> fun)
		: m_fun(fun), m_transform(obj->get<Comp::Transform>()) {
	}

	void PositionFun::update(float time, float dt) {
		m_transform.setPosition(m_fun(time, dt));
	}
}