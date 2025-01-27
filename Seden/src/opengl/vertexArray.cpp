#include <glad/glad.h>
#include "src/opengl/vertexArray.hpp"

namespace Seden {
	VertexArray::VertexArray() {
		glGenVertexArrays(1, &m_id);
		bind();
	}
	VertexArray::~VertexArray() {
		glDeleteVertexArrays(1, &m_id);
	}
	void VertexArray::bind() const {
		glBindVertexArray(m_id);
	}
	void VertexArray::unbind() const {
		glBindVertexArray(0);
	}

	void VertexArray::addVertexBuffer(const VertexBuffer& vertexBuffer, const VertexArrayLayout& layout) {
		bind();
		vertexBuffer.bind();

		uint32_t totalCount = 0;
		uint32_t pos = 0;
		uint32_t offset = 0;
		
		for (uint32_t el : layout.getLayout()) {
			totalCount += el;
		}
		
		for (uint32_t el : layout.getLayout()) {
			glVertexAttribPointer(pos, el, GL_FLOAT, GL_FALSE, totalCount * sizeof(float), (void*)(offset*sizeof(float)));
			glEnableVertexAttribArray(pos);
			offset += el;
			pos++;
		}
	}
}