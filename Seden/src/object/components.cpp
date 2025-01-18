#define STB_TRUETYPE_IMPLEMENTATION
#include "src/object/components.hpp"
#include "src/logger.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glad/glad.h>

namespace Seden {

	// Transform implementation
	Transform::Transform() : m_transform(glm::mat4(1.0f)) {}

	Transform::Transform(const glm::mat4& transform) : m_transform(transform) {}

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
	PolygonMesh::PolygonMesh(const std::initializer_list<Vertex>& mesh) {
		DEBUG_ASSERT(mesh.size() > 2, "Vertex count should be greater than 2");
		for (const Vertex& vertice : mesh) {
			m_vertices.push_back(vertice);
		}
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

	void GroupObjects::addObject(const std::shared_ptr<Object>& object) {
		m_objects.push_back(object);
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

	auto GroupObjects::begin() { return m_objects.begin(); }
	auto GroupObjects::end() { return m_objects.end(); }
	auto GroupObjects::begin() const { return m_objects.begin(); }
	auto GroupObjects::end() const { return m_objects.end(); }

	// Perspective implementation
	Perspective::Perspective(float aspectRatio) : m_projection(glm::perspective(glm::radians(90.f), aspectRatio, 0.01f, 100.f)) {}

	glm::mat4& Perspective::get() {
		return m_projection;
	}

	// TextComp implementation
	bool TextComp::reconstructVB = false;

	TextComp::TextComp() : texResolution(2046), fontPath("c:/windows/fonts/times.ttf"), m_text("") {}

	TextComp::TextComp(std::string text) : texResolution(2046), fontPath("c:/windows/fonts/times.ttf"), m_text(std::move(text)) {}

	void TextComp::setText(const std::string& text) {
		m_text = text;
		reconstructVB = true;
	}

	void TextComp::reloadText() {
		unsigned char* ttf_buffer = (unsigned char*)malloc(1 << 20);
		unsigned char* temp_bitmap = (unsigned char*)malloc(texResolution * texResolution);
	
		FILE* fontFile = fopen(fontPath.string().c_str(), "rb");
		if (!fontFile) {
			DEBUG_MSG("Error: Failed to open font file.");
			return;
		}
		fread(ttf_buffer, 1, 1 << 20, fontFile);
		fclose(fontFile);
	
		stbtt_BakeFontBitmap(ttf_buffer, 0, 64, temp_bitmap, texResolution, texResolution, 32, 96, cdata);
		free(ttf_buffer);
	
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &ftex);
		glBindTexture(GL_TEXTURE_2D, ftex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, texResolution, texResolution, 0, GL_RED, GL_UNSIGNED_BYTE, temp_bitmap);
		free(temp_bitmap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

}
