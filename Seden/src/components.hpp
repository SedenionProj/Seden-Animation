#pragma once

#include <glm/glm.hpp>

namespace Seden {
	class Transform {
	public:
		Transform() = default;
		
		glm::mat4 m_transform = glm::mat4(1);
	};

	class PolygonMesh {
	public:
		PolygonMesh() {

		}

		
	private:

		struct Vertex {
			glm::vec2 position;
		};
	};
}