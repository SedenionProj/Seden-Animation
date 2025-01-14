#pragma once
#include <memory>
//#include "src/object/object.hpp"

namespace Seden {
	class Animation {
	public:
		virtual void update(float time) = 0;
	};

	class MoveTo : public Animation {
	public:
		void update(float time) override;
	};
}