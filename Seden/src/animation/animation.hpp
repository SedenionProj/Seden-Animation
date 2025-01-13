#pragma once
#include <memory>

namespace Seden {
	class Animation {
	public:
		virtual void update(float time) = 0;
	};

}