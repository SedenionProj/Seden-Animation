#include "buffer.hpp"
#include "buffer.hpp"
#include <glad/glad.h>

#include "src/opengl/buffer.hpp"
#include "src/logger.h"

namespace Seden {

	// VertexBuffer

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

	// IndexBuffer

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

	// ShaderStorageBuffer

	uint32_t ShaderStorageBuffer::m_index = 0;

	ShaderStorageBuffer::ShaderStorageBuffer(uint32_t size, const void* data) {
		GLint maxSSBOSize = 0;
		glGetIntegerv(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &maxSSBOSize);
		DEBUG_ASSERT(size <= maxSSBOSize, "Failed to create Shader storage buffer, capacity exceeded (current: %i, max: %i)", size, maxSSBOSize);

		glGenBuffers(1,&m_id);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void ShaderStorageBuffer::bind() {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_index, m_id);
		m_index++;
	}


	// Framebuffer

	Framebuffer::Framebuffer(uint32_t width, uint32_t height)
		: m_width(width), m_height(height) {
		glGenFramebuffers(1, &m_id);
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);

		GLint maxTexSize;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
		DEBUG_ASSERT(width <= (uint32_t)maxTexSize && height <= (uint32_t)maxTexSize, "Framebuffer %u x %u too large (max : %i)", width, height, maxTexSize);

		// Create texture
		glGenTextures(1, &m_colorTex);
		glBindTexture(GL_TEXTURE_2D, m_colorTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Attach texture
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTex, 0);

		GLenum ret_code = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (ret_code != GL_FRAMEBUFFER_COMPLETE) {
			DEBUG_ERROR("Failed to create Framebuffer : %u", ret_code);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}
	
	Framebuffer::~Framebuffer() {
		glDeleteFramebuffers(1, &m_id);
	}

	void Framebuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	}

	void Framebuffer::unBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}