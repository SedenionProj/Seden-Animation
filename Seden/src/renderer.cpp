#include <glad/glad.h>
#include "renderer.hpp"

namespace Seden {
	void Renderer::init()
	{
		gladLoadGL();
	}
}