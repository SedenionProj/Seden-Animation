#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "src/renderer.hpp"
#include "src/components.hpp"

#define MAX_POLYGON_MESH 100000

namespace Seden {
	Renderer::Renderer()
	{
		gladLoadGL();

		polygonMeshVBO = new VertexBuffer(0, nullptr);
		polygonMeshVAO = new VertexArray();
		polygonMeshVAO->addVertexBuffer(*polygonMeshVBO, VertexArrayLayout({
			3, // position
			}));

		polygonMeshIBO = new IndexBuffer();
		polygonMeshIBO->setQuadLayout(1);
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::beginFrame()
	{
		glfwPollEvents();
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void Renderer::endFrame()
	{
		glfwSwapBuffers(glfwGetCurrentContext());
	}

	void Renderer::drawPolygonMesh(const Transform& trasform, const PolygonMesh& mesh)
	{
		// todo batching
		polygonMeshVBO->setData(mesh.getsize(), mesh.getdata());
		polygonMeshVAO->bind();
		polygonMeshIBO->bind();
		glDrawElements(GL_TRIANGLES, polygonMeshIBO->getCount(), GL_UNSIGNED_INT, nullptr);
	}
}