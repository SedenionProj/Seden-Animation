#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Seden {
	class VertexBuffer {
	public:
		VertexBuffer();

		VertexBuffer(size_t size, const void* data);

		VertexBuffer(size_t size);

		~VertexBuffer();

		void bind() const;

		void unbind() const;

		void changeData(size_t size, uint32_t byteOffset, const void* data);

		void setData(size_t size, const void* data);

		void setData(size_t size);
	private:
		uint32_t m_id;
	};

	class IndexBuffer {
	public:
		IndexBuffer();

		IndexBuffer(uint32_t count, uint32_t* indices);

		~IndexBuffer();

		void setQuadLayout(const uint32_t quadNumber);

		void setData(uint32_t count, uint32_t* indices);

		void bind() const;

		void unbind() const;

		uint32_t getCount() const;

	private:
		uint32_t m_count = 0;
		uint32_t m_id;
	};

	class ShaderStorageBuffer {
	public:
		ShaderStorageBuffer(uint32_t size, const void* data);
		void bind();
	private:
		static uint32_t m_index;
		uint32_t m_id;
	};

	class Framebuffer {
	public:
		Framebuffer(uint32_t width, uint32_t height);
		~Framebuffer();

		uint32_t getTexture() { return m_colorTex; }

		void bind();
		void unBind();
	private:
		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_id;
		uint32_t m_colorTex;
	};
}