#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "src/logger.h"

namespace Seden {
	class Transform {
	public:
		Transform() = default;
		Transform(const glm::mat4& transform) : m_transform(transform) {}
		glm::mat4 m_transform = glm::mat4(1);
	};

	class PolygonMesh {
	public:
		struct Vertex {
			glm::vec3 position;
			glm::vec3 color = { 1,1,1 };
		};
	public:
		PolygonMesh(const std::initializer_list<Vertex>& mesh) {
			DEBUG_ASSERT(mesh.size() > 2, "Vertex count should be greater than 2");
			for (const Vertex& vertice : mesh) {
				m_vertices.push_back(vertice);
			}
		}

		void setColor(const glm::vec3& color) {
			for (auto& vertex : m_vertices) {
				vertex.color = color;
			}
		}

		void setColorGradient(const std::initializer_list<glm::vec3>& grad) {
			DEBUG_ASSERT(grad.size() == getVertexCount(), "Gradient color count doesn't match vertex count.");
			auto gradIt = grad.begin();
			for (int i = 0; i < m_vertices.size(); i++) {
				if (gradIt == grad.end()) break;
				m_vertices[i].color = *(gradIt++);
			}
		}

		size_t getsize() const {
			return m_vertices.size() * sizeof(Vertex);
		}

		const Vertex* getdata() const {
			return m_vertices.data();
		}

		uint32_t getVertexCount() const {
			return m_vertices.size();
		}
		
	private:
		std::vector<Vertex> m_vertices;
	};
}