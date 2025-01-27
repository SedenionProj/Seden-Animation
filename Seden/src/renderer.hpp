#pragma once
#include "src/object/components.hpp"
#include "src/opengl/vertexArray.hpp"
#include "src/opengl/buffer.hpp"
#include "src/opengl/shader.hpp"
#include "src/window.hpp"
#include "src/font.hpp"

namespace Seden {
	class PerspectiveCamera;

	class Renderer {
	public:
		Renderer(Window& window);
		~Renderer();

		void setCamera(std::shared_ptr<PerspectiveCamera> camera);

		void beginFrame();
		void endFrame();
		void drawDebugGui();

		void drawConvexPolygon(Comp::Transform& transform, Comp::PolygonMesh& mesh);
		void drawSimpleText(Comp::Transform& transform, Comp::Text& text);
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

	private:
		struct PolygonData
		{
			std::unique_ptr<VertexBuffer> vbo;
			std::unique_ptr<VertexArray>  vao;
			std::unique_ptr<IndexBuffer>  ibo;
			std::unique_ptr<Shader> shader;
			std::vector<Comp::PolygonMesh::Vertex> verticesList;
			std::vector<uint32_t> indicesList;
			uint32_t vertexOffset = 0;
		};

		// text data
		std::unique_ptr<Font> m_font;
		std::unique_ptr<Shader> m_shader;

		PolygonData m_polygonData;

		

		std::shared_ptr<PerspectiveCamera> m_camera;
		Window& m_window;
	};
}