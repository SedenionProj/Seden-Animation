#pragma once
#include <memory>
#include "src/opengl/buffer.hpp"
#include "src/opengl/vertexArray.hpp"
#include "src/opengl/shader.hpp"

namespace Seden {
	class Window;

	enum class ShaderDataType {
		BOOL = 0, INT, FLOAT, VEC2, VEC3, VEC4, MAT2, MAT3, MAT4
	};

	struct Uniform {
		void* data;
		std::string name;
		ShaderDataType dataType;
	};

	class ShaderRenderer {
	public:
		ShaderRenderer() = default;
		void init(Window* window);
		//~ShaderRenderer();
		void beginFrame();
		void endFrame();
		void draw();
		Shader m_shader;
		std::vector<Uniform> m_uniforms;
	private:
		void initImgui();
	private:
		glm::vec2 m_range = {0, 1};
		Window* m_window;
		std::unique_ptr<VertexBuffer> m_vbo;
		std::unique_ptr<VertexArray>  m_vao;
	};
}