#include "src/object/components.hpp"
#include "src/object/object.hpp"
#include "src/opengl/shader.hpp"

namespace Seden {
	namespace Comp {
		// Transform implementation
		Transform::Transform() : m_transform(glm::mat4(1.0f)) {}

		Transform::Transform(const glm::mat4& transform) : m_transform(transform) {}

		Transform::Transform(const glm::vec3& position)
			: Transform() {
			setPosition(position);
		}

		Transform::operator glm::mat4& () {
			return m_transform;
		}

		Transform::operator const glm::mat4& () const {
			return m_transform;
		}

		glm::vec3 Transform::getPosition() const {
			return glm::vec3(m_transform[3]);
		}

		glm::quat Transform::getRotation() const {
			return glm::quat_cast(glm::mat3(m_transform));
		}

		glm::vec3 Transform::getScale() const {
			return glm::vec3(
				glm::length(glm::vec3(m_transform[0])),
				glm::length(glm::vec3(m_transform[1])),
				glm::length(glm::vec3(m_transform[2]))
			);
		}

		glm::mat4& Transform::getTransform() {
			return m_transform;
		}

		const glm::mat4& Transform::getTransform() const {
			return m_transform;
		}

		void Transform::setPosition(const glm::vec3& position) {
			m_transform[3] = glm::vec4(position, 1.0f);
		}

		void Transform::setRotation(const glm::quat& rotation) {
			glm::vec3 scale = getScale();
			glm::mat4 rotMatrix = glm::toMat4(rotation);

			m_transform[0] = glm::vec4(glm::normalize(glm::vec3(rotMatrix[0])) * scale.x, 0.0f);
			m_transform[1] = glm::vec4(glm::normalize(glm::vec3(rotMatrix[1])) * scale.y, 0.0f);
			m_transform[2] = glm::vec4(glm::normalize(glm::vec3(rotMatrix[2])) * scale.z, 0.0f);
		}

		void Transform::setScale(const glm::vec3& scale) {
			glm::vec3 currentScale = getScale();
			if (currentScale.x != 0) m_transform[0] *= scale.x / currentScale.x;
			if (currentScale.y != 0) m_transform[1] *= scale.y / currentScale.y;
			if (currentScale.z != 0) m_transform[2] *= scale.z / currentScale.z;
		}

		void Transform::translate(const glm::vec3& offset) {
			m_transform = glm::translate(m_transform, offset);
		}

		void Transform::rotate(const glm::quat& rotation) {
			m_transform = m_transform * glm::toMat4(rotation);
		}

		void Transform::scale(const glm::vec3& factor) {
			m_transform = glm::scale(m_transform, factor);
		}

		// PolygonMesh implementation
		size_t PolygonMesh::totalVertexCount = 0;
		bool PolygonMesh::hasVertexCountChanged = false;

		PolygonMesh::PolygonMesh(const std::initializer_list<Vertex>& mesh) {
			DEBUG_ASSERT(mesh.size() > 2, "Vertex count should be greater than 2");
			for (const Vertex& vertice : mesh) {
				m_vertices.push_back(vertice);
			}
			totalVertexCount += getVertexCount();
			hasVertexCountChanged = true;
		}

		PolygonMesh::~PolygonMesh()
		{
			totalVertexCount -= getVertexCount();
			hasVertexCountChanged = true;
		}

		void PolygonMesh::setColor(const glm::vec3& color) {
			for (auto& vertex : m_vertices) {
				vertex.color = color;
			}
		}

		void PolygonMesh::setColorGradient(const std::initializer_list<glm::vec3>& grad) {
			DEBUG_ASSERT(grad.size() == getVertexCount(), "Gradient color count doesn't match vertex count.");
			auto gradIt = grad.begin();
			for (int i = 0; i < m_vertices.size(); i++) {
				if (gradIt == grad.end()) break;
				m_vertices[i].color = *(gradIt++);
			}
		}

		PolygonMesh::Vertex& PolygonMesh::getVertex(uint32_t index) {
			return m_vertices[index];
		}

		size_t PolygonMesh::getsize() const {
			return m_vertices.size() * sizeof(Vertex);
		}

		const PolygonMesh::Vertex* PolygonMesh::getdata() const {
			return m_vertices.data();
		}

		size_t PolygonMesh::getVertexCount() const {
			return m_vertices.size();
		}

		std::vector<PolygonMesh::Vertex>& PolygonMesh::getVertices() {
			return m_vertices;
		}

		const std::vector<PolygonMesh::Vertex>& PolygonMesh::getVertices() const {
			return m_vertices;
		}

		// GroupObjects implementation
		GroupObjects::GroupObjects() = default;

		GroupObjects::GroupObjects(std::vector<std::shared_ptr<Object>> objects) : m_objects(std::move(objects)) {}

		GroupObjects::GroupObjects(std::initializer_list<std::shared_ptr<Object>> objects) : m_objects(objects) {}

		GroupObjects::GroupObjects(GroupObjects&& other) noexcept : m_objects(std::move(other.m_objects)) {}

		std::shared_ptr<Object> GroupObjects::addObject(std::shared_ptr<Object> object) {
			m_objects.push_back(object);
			return object;
		}

		void GroupObjects::removeObject(size_t index) {
			if (index < m_objects.size()) {
				m_objects.erase(m_objects.begin() + index);
			}
			else {
				DEBUG_ERROR("Invalid index for removeObject\n");
			}
		}

		std::shared_ptr<Object> GroupObjects::getObject(size_t index) const {
			if (index < m_objects.size()) {
				return m_objects[index];
			}
			else {
				DEBUG_ERROR("Invalid index for getObject\n");
				return nullptr;
			}
		}

		size_t GroupObjects::size() const {
			return m_objects.size();
		}

		void GroupObjects::clear() {
			m_objects.clear();
		}


		// Perspective implementation
		Perspective::Perspective(float aspectRatio) : m_projection(glm::perspective(glm::radians(90.f), aspectRatio, 0.01f, 100.f)) {}

		glm::mat4& Perspective::get() {
			return m_projection;
		}

		// Orthographic implementation
		Orthographic::Orthographic(float aspectRatio) : m_projection(glm::ortho(-aspectRatio,aspectRatio, -1.f,1.f,-1.f,1.f)) {}

		Orthographic::Orthographic(float left, float right, float bottom, float top) : m_projection(glm::ortho(left, right, bottom, top, -1.f, 1.f)) {}

		glm::mat4& Orthographic::get() {
			return m_projection;
		}


		Text::Text(std::string text, Comp::GroupObjects* letters, float scale, const glm::vec4& color)
			:m_text(text), m_letters(letters), m_scale(scale), m_defaultColor(color) {
			reloadText();
		};

		// Text implementation
		void Text::reloadText() {
			m_letters->clear();
			for (char c : m_text) {
                if (!(32 <= c && c < 128 && c != ' ')) continue;
				auto obj = Object::create();
				obj->add<Comp::Transform>(glm::mat4(1));
				obj->add<Comp::Color>(m_defaultColor);
				m_letters->addObject(obj);
			}
		}

		size_t Point::totalVertexCount = 0;
		bool Point::hasVertexCountChanged = false;

		Point::Point(float thickness) 
			: m_thickness(thickness) {
			totalVertexCount += 1;
			hasVertexCountChanged = true;
		}

		Point::~Point() {
			totalVertexCount -= 1;
			hasVertexCountChanged = true;
		}

		// Shader implementation
		Shader::Shader(uint32_t instanceCount, std::shared_ptr<Seden::Shader> shader)
			: m_shader(shader) {}

		// Path
		size_t Path::totalVertexCount = 0;
		size_t Path::totalLineCount= 0;
		bool Path::hasVertexCountChanged = false;

		Path::Path(const std::vector<Vertex>& vertices)
			: m_vertices(vertices) {
			hasVertexCountChanged = true;
			totalVertexCount += vertices.size();
			totalLineCount += vertices.size() - 3;
		}


		Path::Path(const std::initializer_list<Comp::Path::Vertex> vertices)
			: m_vertices(vertices) {
			hasVertexCountChanged = true;
			totalVertexCount += vertices.size();
			totalLineCount += vertices.size() - 3;
		}

		
	}
}
