#pragma once

namespace Seden {
	class Curve {
	public:
		virtual float get(float time) const = 0;

		float operator()(float time) const;
	};

	class EaseIn : public Curve{
	public:
		EaseIn(float strength);

		float get(float time) const override;

	private:
		float m_strength;
	};

	class EaseInOut : public Curve {
	public:
		EaseInOut(float strength);

		float get(float time) const override;

	private:
		float m_strength;
	};
}