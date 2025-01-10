#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace Seden {
	class Transform {
	public:
		Transform() = default;
		
		glm::mat4 m_transform = glm::mat4(1);
	};

	class PolygonMesh {
	public:
		struct Vertex {
			glm::vec2 position;
		};
	public:
		PolygonMesh() {
			// quad
			vertices.push_back({ glm::vec2(-0.5f, -0.5f) });
			vertices.push_back({ glm::vec2(0.5f, -0.5f) });
			vertices.push_back({ glm::vec2(0.5f, 0.5f) });
			vertices.push_back({ glm::vec2(-0.5f, 0.5f) });
		}

		size_t getsize() const {
			return vertices.size() * sizeof(Vertex);
		}

		const Vertex* getdata() const {
			return vertices.data();
		}
		
	private:
		std::vector<Vertex> vertices;

	};
}