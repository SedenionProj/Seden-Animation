#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "src/logger.h"

namespace Seden {
    class Object;

    class Transform {
    public:
        Transform() = default;
        Transform(const glm::mat4& transform) : m_transform(transform) {}

        operator glm::mat4& () {
            return m_transform;
        }

        operator const glm::mat4& () const {
            return m_transform;
        }

        // Getters
        glm::vec3 getPosition() const {
            return glm::vec3(m_transform[3]);
        }

        glm::quat getRotation() const {
            return glm::quat_cast(glm::mat3(m_transform));
        }

        glm::vec3 getScale() const {
            return glm::vec3(
                glm::length(glm::vec3(m_transform[0])),
                glm::length(glm::vec3(m_transform[1])),
                glm::length(glm::vec3(m_transform[2]))
            );
        }

        glm::mat4& getTransform() {
            return m_transform;
        }

        const glm::mat4& getTransform() const {
            return m_transform;
        }

        // Setters
        void setPosition(const glm::vec3& position) {
            m_transform[3] = glm::vec4(position, 1.0f);
        }

        void setRotation(const glm::quat& rotation) {
            glm::vec3 scale = getScale();
            glm::mat4 rotMatrix = glm::toMat4(rotation);

            m_transform[0] = glm::vec4(glm::normalize(glm::vec3(rotMatrix[0])) * scale.x, 0.0f);
            m_transform[1] = glm::vec4(glm::normalize(glm::vec3(rotMatrix[1])) * scale.y, 0.0f);
            m_transform[2] = glm::vec4(glm::normalize(glm::vec3(rotMatrix[2])) * scale.z, 0.0f);
        }

        void setScale(const glm::vec3& scale) {
            glm::vec3 currentScale = getScale();
            if (currentScale.x != 0) m_transform[0] *= scale.x / currentScale.x;
            if (currentScale.y != 0) m_transform[1] *= scale.y / currentScale.y;
            if (currentScale.z != 0) m_transform[2] *= scale.z / currentScale.z;
        }

        // Utility functions
        void translate(const glm::vec3& offset) {
            m_transform = glm::translate(m_transform, offset);
        }

        void rotate(const glm::quat& rotation) {
            m_transform = m_transform * glm::toMat4(rotation);
        }

        void scale(const glm::vec3& factor) {
            m_transform = glm::scale(m_transform, factor);
        }

    private:
        glm::mat4 m_transform = glm::mat4(1.0f);
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

        // Setters
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

        // Getters

		Vertex& getVertex(uint32_t index) {
			return m_vertices[index];
		}

		size_t getsize() const {
			return m_vertices.size() * sizeof(Vertex);
		}

		const Vertex* getdata() const {
			return m_vertices.data();
		}

		size_t getVertexCount() const {
			return m_vertices.size();
		}
		
        std::vector<Vertex>& getVertices() {
            return m_vertices;
        }

        const std::vector<Vertex>& getVertices() const {
            return m_vertices;
        }
	private:
		std::vector<Vertex> m_vertices;
	};

    class GroupObjects {
    public:
		
		GroupObjects() = default;

		explicit GroupObjects(std::vector<std::shared_ptr<Object>> objects)
			: m_objects(std::move(objects)) {
		}

		GroupObjects(std::initializer_list<std::shared_ptr<Object>> objects)
			: m_objects(objects) {
		}

		GroupObjects(GroupObjects&& other) noexcept
			: m_objects(std::move(other.m_objects)) {
		}

		GroupObjects(const GroupObjects&) = delete;

		void addObject(const std::shared_ptr<Object>& object) {
			m_objects.push_back(object);
		}

		void removeObject(size_t index) {
			if (index < m_objects.size()) {
				m_objects.erase(m_objects.begin() + index);
			}
			else {
				DEBUG_ERROR("Invalid index for removeObject\n");
			}
		}

		std::shared_ptr<Object> getObject(size_t index) const {
			if (index < m_objects.size()) {
				return m_objects[index];
			}
			else {
				DEBUG_ERROR("Invalid index for getObject\n");
				return nullptr;
			}
		}

		size_t size() const {
			return m_objects.size();
		}

		void clear() {
			m_objects.clear();
		}

		auto begin() { return m_objects.begin(); }
		auto end() { return m_objects.end(); }
		auto begin() const { return m_objects.begin(); }
		auto end() const { return m_objects.end(); }
    private:
        std::vector<std::shared_ptr<Object>> m_objects;
    };

	class Perspective {
	public:
		Perspective(float aspectRatio)
			: m_projection(glm::perspective(glm::radians(90.f), aspectRatio, 0.01f, 100.f)) {}

		glm::mat4& get() { return m_projection; }

	private:
		glm::mat4 m_projection;
	};
}