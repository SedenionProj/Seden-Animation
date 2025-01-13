#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "src/renderer.hpp"
#include "src/components.hpp"
#include "src/logger.h"

#define MAX_POLYGON_MESH 100000

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

	Renderer::Renderer()
	{
		gladLoadGL();
		
#if defined(DEBUG) || defined(_DEBUG)
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallback, 0);
#endif

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
	}

	void Renderer::beginFrame()
	{
		glfwPollEvents();
		glClearColor(0.1f,0.1f,0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void Renderer::endFrame()
	{
		glfwSwapBuffers(glfwGetCurrentContext());
	}

	void Renderer::drawPolygonMesh(Transform& transform, PolygonMesh& mesh)
	{
		// todo batching
		std::vector<PolygonMesh::Vertex> transformMesh;
		for (auto& vertex : mesh.getVertices()) {
			transformMesh.emplace_back(PolygonMesh::Vertex(glm::vec3(transform.getTransform() * glm::vec4(vertex.position, 1)), vertex.color));
		}

		shader->Bind();
		polygonMeshVBO->setData(mesh.getsize(), transformMesh.data());
		polygonMeshVAO->bind();
		glDrawArrays(GL_TRIANGLE_FAN, 0, mesh.getVertexCount());
	}
}