#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Seden {
	class VertexBuffer {
	public:
		VertexBuffer() {
			glGenBuffers(1, &m_id);
			glBindBuffer(GL_ARRAY_BUFFER, m_id);
		}

		VertexBuffer(size_t size, const void* data) {
			VertexBuffer();
			setData(size, data);
		}

		VertexBuffer(size_t size) {
			VertexBuffer(size, nullptr);
		}

		~VertexBuffer() {
			glDeleteBuffers(1, &m_id);
		}

		void bind() const {
			glBindBuffer(GL_ARRAY_BUFFER, m_id);
		}

		void unbind() const {
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void changeData(size_t size, uint32_t byteOffset, const void* data) {
			bind();
			glBufferSubData(GL_ARRAY_BUFFER, byteOffset, size,data);
		}

		void setData(size_t size, const void* data) {
			bind();
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
		}

		void setData(size_t size) { setData(size, nullptr); }
	private:
		GLuint m_id;
	};

	class IndexBuffer {
	public:
		IndexBuffer() {
			glGenBuffers(1, &m_id);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		}

		IndexBuffer(uint32_t count, uint32_t* indices) {
			IndexBuffer();
			data(count, indices);
		}

		~IndexBuffer() {
			glDeleteBuffers(GL_ELEMENT_ARRAY_BUFFER, &m_id);
		}

		void setQuadLayout(const uint32_t quadNumber){
			std::vector<uint32_t> indices(quadNumber * 6);
			for (int i = 0; i < quadNumber; i++) {
				indices[6*i+0] = 4*i+0;
				indices[6*i+1] = 4*i+1;
				indices[6*i+2] = 4*i+2;
				indices[6*i+3] = 4*i+2;
				indices[6*i+4] = 4*i+3;
				indices[6*i+5] = 4*i+0;
			}

			data(quadNumber*6, indices.data());
		}

		void data(uint32_t count, uint32_t* indices) {
			m_count = count;
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, indices, GL_STATIC_DRAW);
		}

		void bind() const {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		}

		void unbind() const {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		uint32_t getCount() const { return m_count; }

	private:
		uint32_t m_count = 0;
		GLuint m_id;
	};
}