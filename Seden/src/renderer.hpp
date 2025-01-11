#pragma once
#include "src/components.hpp"
#include "src/opengl/vertexArray.hpp"
#include "src/opengl/buffer.hpp"
#include "src/opengl/shader.hpp"

namespace Seden {

	class Renderer {
	public:
		Renderer();
		~Renderer();

		void beginFrame();
		void endFrame();

		void drawPolygonMesh(const Transform& trasform, const PolygonMesh& mesh);

	private:
		VertexBuffer* polygonMeshVBO;
		VertexArray* polygonMeshVAO;
		Shader* shader;
	};
}