#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "src/renderer.hpp"
#include "src/object/components.hpp"
#include "src/logger.h"
#include "src/object/object.hpp"

namespace Seden {
	static void GLAPIENTRY MessageCallback(GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam)
	{
		fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			type, severity, message);
	}

	Renderer::Renderer(Window& window)
		:m_window(window)
	{
		gladLoadGL();
		
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallback, 0);
#if defined(DEBUG) || defined(_DEBUG)
		//glEnable(GL_DEBUG_OUTPUT);
		//glDebugMessageCallback(MessageCallback, 0);
#endif
		polygonMeshIBO = new IndexBuffer();
		polygonMeshVBO = new VertexBuffer();
		polygonMeshVAO = new VertexArray();
		polygonMeshVAO->addVertexBuffer(*polygonMeshVBO, VertexArrayLayout({
			3, // position
			3, // color
		}));
		shader = new Shader();
		shader->createShader(baseVertexShader, baseFragmentShader);
	}

	Renderer::~Renderer()
	{
		//delete buffers
	}

	void Renderer::setCamera(std::shared_ptr<PerspectiveCamera> camera)
	{
		m_camera = camera;
	}

	void Renderer::beginFrame()
	{
		glfwPollEvents();
		glClearColor(0.1f,0.1f,0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		if (PolygonMesh::hasVertexCountChanged) {
			std::cout << "polygonMeshVBO size " << PolygonMesh::totalVertexCount<<"\n";
			polygonMeshIndices.reserve(PolygonMesh::totalVertexCount);
			m_vertex.resize(PolygonMesh::totalVertexCount);
		}
	}

	void Renderer::endFrame()
	{
		if (PolygonMesh::totalVertexCount) {
			if (PolygonMesh::hasVertexCountChanged) {
				polygonMeshIBO->setData(polygonMeshIndices.size(), polygonMeshIndices.data());
				PolygonMesh::hasVertexCountChanged = false;
			}
			polygonMeshVBO->setData(PolygonMesh::totalVertexCount * sizeof(PolygonMesh::Vertex), m_vertex.data());

			polygonMeshOffset = 0;
			shader->Bind();
			shader->setMat4("view", m_camera->getView());
			shader->setMat4("proj", m_camera->getProjection());
			
			polygonMeshIBO->bind();
			polygonMeshVAO->bind();
			glDrawElements(GL_TRIANGLES, polygonMeshIBO->getCount(), GL_UNSIGNED_INT, nullptr);
		}

		glfwSwapBuffers(m_window.getWindowPtr());
	}

	void Renderer::drawPolygonMesh(Transform& transform, PolygonMesh& mesh)
	{
		// todo: skip if Vertex has not changed, multiple batches
		const size_t n = mesh.getVertexCount();

		if (PolygonMesh::hasVertexCountChanged) {
			for (int i = 1; i < n - 1; i++) {
				polygonMeshIndices.emplace_back(polygonMeshOffset);
				polygonMeshIndices.emplace_back(polygonMeshOffset + i);
				polygonMeshIndices.emplace_back(polygonMeshOffset + i + 1);
			}
		}
		
		for (int i = 0; i < n; i++) {
			const auto& meshVertex = mesh.getVertex(i);
			m_vertex[polygonMeshOffset+i].position = transform.getTransform() * glm::vec4(meshVertex.position, 1);
			m_vertex[polygonMeshOffset+i].color = meshVertex.color;
		}

		//polygonMeshVBO->changeData(n * sizeof(PolygonMesh::Vertex),
		//	polygonMeshOffset * sizeof(PolygonMesh::Vertex),
		//	m_vertex.data());
		//

		polygonMeshOffset += n;
	}
}
