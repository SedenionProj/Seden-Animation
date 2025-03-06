#pragma once
#include "src/object/components.hpp"
#include "src/opengl/vertexArray.hpp"
#include "src/opengl/buffer.hpp"
#include "src/opengl/shader.hpp"
#include "src/font.hpp"

namespace Seden {
	class Camera;
	class Window;

	struct PointVertex {
		glm::vec4 color;
		glm::vec3 position;
		float thickness;
	};

	class Renderer {
	public:
		Renderer() = default;
		void init(Window* window);
		~Renderer();

		void setCamera(std::shared_ptr<Camera> camera);

		void beginFrame();
		void endFrame();
		void drawDebugGui();

		void drawConvexPolygon(Comp::Transform& transform, Comp::PolygonMesh& mesh);
		void drawText(Comp::Transform& transform, Comp::GroupObjects& letters, Comp::Text& text);
		void drawPoint(Comp::Transform& transform, Comp::Color& color, Comp::Point& point);
		void drawShaderQuad(Comp::Transform& transform, Comp::Shader& shader);
	public:
		struct DebugStats
		{
			float frameTimeElapsed = 0;
			uint32_t animationCount = 0;
			uint32_t objectAliveCount = 0;
		};

		DebugStats m_stats;

	private:
		void initImgui();
		void bindCamera(Shader* shader);

	private:
		struct PolygonData {
			std::unique_ptr<VertexBuffer> vbo;
			std::unique_ptr<VertexArray>  vao;
			std::unique_ptr<IndexBuffer>  ibo;
			std::unique_ptr<Shader> shader;
			std::vector<Comp::PolygonMesh::Vertex> verticesList;
			std::vector<uint32_t> indicesList;
			uint32_t vertexOffset = 0;
		};

		struct PointData {
			std::unique_ptr<VertexBuffer> vbo;
			std::unique_ptr<VertexArray>  vao;
			std::unique_ptr<Shader> shader;
			std::vector<PointVertex> verticesList;
			uint32_t vertexOffset = 0;
		};

		// text data
		std::unique_ptr<Font> m_font;
		std::unique_ptr<Shader> m_shader;
		std::unique_ptr<Shader> m_shaderPoint;

		PolygonData m_polygonData;
		PointData m_pointData;

		

		std::shared_ptr<Camera> m_camera;
		Window* m_window;
	};
}