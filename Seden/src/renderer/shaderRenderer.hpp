#pragma once
#include <memory>
#include "src/opengl/buffer.hpp"
#include "src/opengl/vertexArray.hpp"
#include "src/opengl/shader.hpp"
#include "src/font.hpp"
#include "src/object/components.hpp"

namespace Seden {
	class Window;

	enum class ShaderDataType {
		BOOL = 0, INT, FLOAT, VEC2, VEC3, VEC4, MAT2, MAT3, MAT4
	};

	struct Uniform {
		void* data;
		std::string name;
		ShaderDataType dataType;
		glm::vec3 range;
	};

	class ShaderRenderer {
	public:
		ShaderRenderer() = default;
		//~ShaderRenderer();
		void init(Window* window);
		void beginFrame();
		void endFrame();
		void draw(float time, float dt);
		void drawText(Comp::Transform& transform, Comp::GroupObjects& letters, Comp::Text& text);
		Shader m_shader;
		std::vector<Uniform> m_uniforms;
		int m_renderPassCount = 1;
	private:
		void initImgui();
	private:
		Window* m_window;
		Shader m_screenShader;

		// text
		std::unique_ptr<Font> m_font;
		std::unique_ptr<Shader> m_letterShader;

		std::unique_ptr<VertexBuffer> m_vbo;
		std::unique_ptr<Framebuffer> m_framebuffer;
		std::unique_ptr<VertexArray>  m_vao;
		std::unique_ptr<ShaderStorageBuffer> m_colorSSBO;
		std::unique_ptr<ShaderStorageBuffer> m_arraySSBO;
	};
}