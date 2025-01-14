#pragma once
#include <memory>

namespace Seden {
	class Animator {
	public:
		virtual void update(float time) = 0;
	};

	class MoveTo : public Animator {
	public:
		void update(float time) override;
	};
}