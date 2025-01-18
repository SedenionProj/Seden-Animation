#pragma once
#include "src/object/components.hpp"
#include "src/opengl/vertexArray.hpp"
#include "src/opengl/buffer.hpp"
#include "src/opengl/shader.hpp"
#include "src/window.hpp"

namespace Seden {
	class PerspectiveCamera;

	class Renderer {
	public:
		Renderer(Window& window);
		~Renderer();

		void setCamera(std::shared_ptr<PerspectiveCamera> camera);

		void beginFrame();
		void endFrame();

		void drawPolygonMesh(Transform& transform, PolygonMesh& mesh);

	private:
		std::shared_ptr<PerspectiveCamera> m_camera;

		VertexBuffer* polygonMeshVBO;
		VertexArray* polygonMeshVAO;
		IndexBuffer* polygonMeshIBO;
		std::vector<uint32_t> polygonMeshIndices;
		uint32_t polygonMeshOffset = 0;
		Shader* shader;

		Window& m_window;
	};
}