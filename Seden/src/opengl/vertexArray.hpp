#pragma once
#include "src/opengl/buffer.hpp"

namespace Seden {
	class VertexArrayLayout {
	public:
		VertexArrayLayout(std::initializer_list<float> vertexAttribs)
			: vertexAttribs(vertexAttribs) {}
		// todo: data types support
		const std::vector<float>& getLayout() const { return vertexAttribs; }
	private:
		std::vector<float> vertexAttribs;
	};

	class VertexArray {
	public:
		VertexArray();
		~VertexArray();
		void bind() const;
		void unbind() const;
		void addVertexBuffer(const VertexBuffer& vertexBuffer, const VertexArrayLayout& layout);
	private:
		uint32_t m_id;
	};
}