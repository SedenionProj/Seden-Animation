#pragma once
#include "src/object/components.hpp"
#include "src/opengl/vertexArray.hpp"
#include "src/opengl/buffer.hpp"
#include "src/opengl/shader.hpp"
#include "src/window.hpp"

namespace Seden {

	class Renderer {
	public:
		Renderer(Window& window);
		~Renderer();

		void beginFrame();
		void endFrame();

		void drawPolygonMesh(Transform& transform, PolygonMesh& mesh);

	private:
		VertexBuffer* polygonMeshVBO;
		VertexArray* polygonMeshVAO;
		Shader* shader;

		Window& m_window;
	};
}