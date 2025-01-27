#include <glad/glad.h>

#include "src/opengl/buffer.hpp"

namespace Seden {
	VertexBuffer::VertexBuffer()
	{
		glGenBuffers(1, &m_id);
		bind();
	}
	VertexBuffer::VertexBuffer(size_t size, const void* data)
		: VertexBuffer()
	{
		setData(size, data);
	}

	VertexBuffer::VertexBuffer(size_t size) 
		: VertexBuffer(size, nullptr) {}

	VertexBuffer::~VertexBuffer() {
		glDeleteBuffers(1, &m_id);
	}

	void VertexBuffer::bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}

	void VertexBuffer::unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::changeData(size_t size, uint32_t byteOffset, const void* data) {
		bind();
		glBufferSubData(GL_ARRAY_BUFFER, byteOffset, size, data);
	}

	void VertexBuffer::setData(size_t size, const void* data) {
		bind();
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
	}

	void VertexBuffer::setData(size_t size) {
		setData(size, nullptr);
	}

	IndexBuffer::IndexBuffer() {
		glGenBuffers(1, &m_id);
		bind();
	}

	IndexBuffer::IndexBuffer(uint32_t count, uint32_t* indices) 
		: IndexBuffer()
	{
		setData(count, indices);
	}

	IndexBuffer::~IndexBuffer() {
		glDeleteBuffers(1, &m_id);
	}

	void IndexBuffer::setQuadLayout(const uint32_t quadNumber) {
		std::vector<uint32_t> indices(quadNumber * 6);
		for (int i = 0; i < quadNumber; i++) {
			indices[6 * i + 0] = 4 * i + 0;
			indices[6 * i + 1] = 4 * i + 1;
			indices[6 * i + 2] = 4 * i + 2;
			indices[6 * i + 3] = 4 * i + 2;
			indices[6 * i + 4] = 4 * i + 3;
			indices[6 * i + 5] = 4 * i + 0;
		}
		setData(quadNumber * 6, indices.data());
	}

	void IndexBuffer::setData(uint32_t count, uint32_t* indices) {
		bind();
		m_count = count;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, indices, GL_STATIC_DRAW);
	}

	void IndexBuffer::bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	}

	void IndexBuffer::unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	uint32_t IndexBuffer::getCount() const {
		return m_count;
	}
}