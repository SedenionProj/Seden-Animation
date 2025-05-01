#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "src/renderer/renderer.hpp"
#include "src/object/components.hpp"
#include "src/logger.h"
#include "src/object/object.hpp"
#include "src/window.hpp"

//path
inline const char* pathFragmentShader = R"(#version 460

out vec4 fragColor;

in vec2 texCoord;

in vec4 color;

void main() {
	fragColor = color;
}
)";

inline const char* pathVertexShader = R"(#version 460

struct Vertex {
	vec4 color;
	vec4 position;
	float thickness;
	int beginIndex;
	vec2 padding;
};

layout(std430, binding = 0) buffer TVertex
{
   Vertex vertex[]; 
};

uniform float u_thickness;
uniform mat4 view;
uniform mat4 proj;

out vec2 texCoord;
out vec4 color;

void main()
{
    // borrowed from https://stackoverflow.com/questions/3484260/opengl-line-width
	int line_vert_id = gl_VertexID / 6;
    int line_i = vertex[line_vert_id].beginIndex;
    int tri_i  = gl_VertexID % 6;

    

    vec4 va[4];
    for (int i=0; i<4; ++i)
    {
        va[i] = proj*view*vertex[line_i+i].position;
        va[i].xyz /= abs(va[i].w);
    }

    vec2 v_line  = normalize(va[2].xy - va[1].xy);
    vec2 nv_line = vec2(-v_line.y, v_line.x);

    vec4 pos;
    if (tri_i == 0 || tri_i == 1 || tri_i == 3)
    {
        color = vertex[line_i+1].color;

        vec2 v_pred  = normalize(va[1].xy - va[0].xy);
        vec2 v_miter = normalize(nv_line + vec2(-v_pred.y, v_pred.x));

        pos = va[1];

        float angle = dot(v_miter, nv_line);
		
		float thickness = vertex[line_i+1].thickness;

        pos.xy += 0.001*v_miter * thickness * (tri_i == 1 ? -0.5 : 0.5) / max(angle, 0.2); // temp miter fix 
    }
    else
    {
        color = vertex[line_i+2].color;

        vec2 v_succ  = normalize(va[3].xy - va[2].xy);
        vec2 v_miter = normalize(nv_line + vec2(-v_succ.y, v_succ.x));

        pos = va[2];

        float angle = dot(v_miter, nv_line);

		float thickness = vertex[line_i+2].thickness;

        pos.xy += 0.001*v_miter * thickness * (tri_i == 5 ? 0.5 : -0.5) / max(angle, 0.2);
    }

    

    pos.xyz *= abs(pos.w);
    gl_Position = pos;

    texCoord = pos.xy;
}
)";

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

	void Renderer::init(Window* window)
	{
		m_window = window;
		gladLoadGL();
		
		initImgui();

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallback, 0);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glEnable(GL_PROGRAM_POINT_SIZE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// polygon
		m_polygonData.vao= std::make_unique<VertexArray>();
		m_polygonData.ibo = std::make_unique<IndexBuffer>();
		m_polygonData.vbo = std::make_unique<VertexBuffer>();
		m_polygonData.shader = std::make_unique<Shader>();
		m_polygonData.shader->createShader(baseVertexShader, baseFragmentShader);
		m_polygonData.vao->addVertexBuffer(*m_polygonData.vbo, VertexArrayLayout({
			3, // position
			3, // color
		}));

		// text
		m_shader = std::make_unique<Shader>();
		m_shader->createShader(letterVertexShader, letterFragmentShader);

		m_font = std::make_unique<Font>();

		// point
		m_pointData.vao = std::make_unique<VertexArray>();
		m_pointData.vbo = std::make_unique<VertexBuffer>();
		m_pointData.shader = std::make_unique<Shader>();
		m_pointData.shader->createShader(pointVertexShader, pointFragmentShader);
		m_pointData.vao->addVertexBuffer(*m_pointData.vbo, VertexArrayLayout({
			4, // color
			3, // position
			1, // thickness
			}));

		// screen
		m_screenShader.createShader(screenVertexShader, screenFragmentShader);
		m_framebuffer = std::make_unique<Framebuffer>(m_window->getWidth(), m_window->getHeight());

		float vertices[] = {
			-1,  1,
			 1,  1,
			 1, -1,
			-1, -1
		};
		m_screenQuadVAO = std::make_unique<VertexArray>();
		m_vbo = std::make_unique<VertexBuffer>(sizeof(vertices), static_cast<void*>(vertices));
		m_screenQuadVAO->addVertexBuffer(*m_vbo, { 2 });

		// path
		m_pathData.shader = std::make_unique<Shader>(pathVertexShader, pathFragmentShader);
		//m_pathData.verticesList = {
		//	{{1,0,0,1}, {-1  ,0.0,0,1}, 50.f, 0},
		//	{{0,0,1,1}, {-0.5,0,0,  1},	50.f, 4},
		//	{{1,0,1,1}, {-0.3,0.8,0,1}, 50.f, 0},
		//	{{0,1,0,1}, {-0.2,0.8,0,1}, 50.f, 0},
		//
		//	{{0,1,0,1}, { 0.2,0.8,0,1}, 50.f, 0},
		//	{{1,0,0,1}, { 0.3,0.8,0,1}, 50.f, 0},
		//	{{1,1,0,1}, { 0.5,0,0,1},	50.f, 0},
		//	{{1,0,0,1}, { 1  ,0,0,1},	50.f, 0},
		//};
		m_pathData.vao = std::make_unique<VertexArray>();
		m_pathData.ssbo = std::make_unique<ShaderStorageBuffer>(m_pathData.verticesList.size()*sizeof(LineVertex), m_pathData.verticesList.data());
		m_pathData.ssbo->bind(0);
	}

	Renderer::~Renderer()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Renderer::setCamera(std::shared_ptr<Camera> camera)
	{
		m_camera = camera;
	}

	void Renderer::initImgui()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;


		ImGui_ImplGlfw_InitForOpenGL(m_window->getWindowPtr(), true);
		ImGui_ImplOpenGL3_Init("#version 330");
	}

	void Renderer::bindCamera(Shader* shader)
	{
		if (m_camera) {
			shader->bind();
			shader->setMat4("view", m_camera->getView());
			shader->setMat4("proj", m_camera->getProjection());
		}
		
	}

	void Renderer::beginFrame()
	{
		glfwPollEvents();
		m_framebuffer->bind();
		glClearColor(0,0,0, 1.0f);
		//glClearColor(0.1f,0.1f,0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// imgui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (Comp::PolygonMesh::hasVertexCountChanged) {
			m_polygonData.indicesList.clear();
			m_polygonData.indicesList.reserve(Comp::PolygonMesh::totalVertexCount);
			m_polygonData.verticesList.resize(Comp::PolygonMesh::totalVertexCount);
		}

		if (Comp::Point::hasVertexCountChanged) {
			m_pointData.verticesList.resize(Comp::Point::totalVertexCount);
		}

		if (Comp::Path::hasVertexCountChanged) {
			m_pathData.verticesList.resize(Comp::Path::totalVertexCount);
		}
	}

	void Renderer::endFrame()
	{
		if (Comp::Path::totalVertexCount) {
			//reset
			if (Comp::Path::hasVertexCountChanged) {
				Comp::Path::hasVertexCountChanged = false;
			}
			m_pathData.vertexOffset = 0;
			m_pathData.pathOffset = 0;

			//drawing
			m_pathData.ssbo->setData(m_pathData.verticesList.size() * sizeof(LineVertex), m_pathData.verticesList.data());
			m_pathData.vao->bind();
			m_pathData.shader->bind();
			m_pathData.shader->setMat4("view", m_camera->getView());
			m_pathData.shader->setMat4("proj", m_camera->getProjection());
			glDrawArrays(GL_TRIANGLES, 0, Comp::Path::totalLineCount*6);
		}

		if (Comp::PolygonMesh::totalVertexCount) {
			//reset
			if (Comp::PolygonMesh::hasVertexCountChanged) {
				m_polygonData.ibo->setData(m_polygonData.indicesList.size(), m_polygonData.indicesList.data());
				Comp::PolygonMesh::hasVertexCountChanged = false;
			}
			m_polygonData.vbo->setData(Comp::PolygonMesh::totalVertexCount * sizeof(Comp::PolygonMesh::Vertex), m_polygonData.verticesList.data());
			m_polygonData.vertexOffset = 0;

			//drawing
			m_polygonData.shader->bind();
			m_polygonData.shader->setMat4("view", m_camera->getView());
			m_polygonData.shader->setMat4("proj", m_camera->getProjection());
		
			m_polygonData.vao->bind();
			glDrawElements(GL_TRIANGLES, m_polygonData.ibo->getCount(), GL_UNSIGNED_INT, nullptr);
		}

		if (Comp::Point::totalVertexCount) {
			if (Comp::Point::hasVertexCountChanged) {
				Comp::Point::hasVertexCountChanged = false;
			}
			m_pointData.vbo->setData(Comp::Point::totalVertexCount * sizeof(PointVertex), m_pointData.verticesList.data());

			m_pointData.vertexOffset = 0;
			m_pointData.shader->bind();
			m_pointData.shader->setMat4("view", m_camera->getView());
			m_pointData.shader->setMat4("proj", m_camera->getProjection());

			m_pointData.vao->bind();
			glDrawArrays(GL_POINTS, 0, Comp::Point::totalVertexCount);
		}

		m_window->saveFrame();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// draw on screen
		m_framebuffer->unBind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_screenShader.bind();
		m_screenQuadVAO->bind();
		glBindTexture(GL_TEXTURE_2D, m_framebuffer->getTexture());

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		m_window->swapBuffers();
	}

	void Renderer::drawDebugGui() {
		ImGui::Begin("debug");
		ImGui::Text("ms %f", m_stats.frameTimeElapsed);
		ImGui::Text("fps %f", 1.f/m_stats.frameTimeElapsed);
		ImGui::Text("animation count %d", m_stats.animationCount);
		ImGui::Text("object alive count %d", m_stats.objectAliveCount);
		ImGui::End();
	}

	void Renderer::drawPath(Comp::Path& path) {
		const size_t n = path.m_vertices.size();

		for (int i = 0; i < n; i++) {
			const auto& vert = path.m_vertices[i];
			auto& vertex = m_pathData.verticesList[m_pathData.vertexOffset + i];

			if (i < n - 3) {
				// for each line, add beginIndex
				m_pathData.verticesList[m_pathData.pathOffset].beginIndex = m_pathData.vertexOffset +i;
				m_pathData.pathOffset += 1;
			}
			

			vertex.position = glm::vec4(vert.position, 1);
			vertex.color = vert.color;
			vertex.thickness = vert.thickness;
		}

		m_pathData.vertexOffset += n;
	}

	void Renderer::drawConvexPolygon(Comp::Transform& transform, Comp::PolygonMesh& mesh)
	{
		// todo: skip if Vertex has not changed, multiple batches, reserve the right amount of indices
		const size_t n = mesh.getVertexCount();

		if (Comp::PolygonMesh::hasVertexCountChanged) {
			for (int i = 1; i < n - 1; i++) {
				m_polygonData.indicesList.emplace_back(m_polygonData.vertexOffset);
				m_polygonData.indicesList.emplace_back(m_polygonData.vertexOffset + i);
				m_polygonData.indicesList.emplace_back(m_polygonData.vertexOffset + i + 1);
			}
		}


		for (int i = 0; i < n; i++) {
			const auto& meshVertex = mesh.getVertex(i);
			auto& vertex = m_polygonData.verticesList[m_polygonData.vertexOffset + i];
			vertex.position = transform.getTransform() * glm::vec4(meshVertex.position, 1);
			vertex.color = meshVertex.color;
		}

		m_polygonData.vertexOffset += n;
	}

	struct LetterVertex {
		glm::vec4 color;
		glm::vec3 position;
		glm::vec2 texCoord;
	};

	void Renderer::drawText(Comp::Transform& transform, Comp::GroupObjects& letters, Comp::Text& text)
	{
		float x = 0, y = 0;
		uint32_t i = 0;
		float scale = m_font->fontSize/text.getScale();
		float lineSpace = 1;
		float lineSkip = 0;
		float center = 0;
		stbtt_aligned_quad q;
		char* letter = (char*)text.getText().c_str();
        auto it = letters.begin();
		while (*letter) {
			
			if (*letter == '\n') {
				lineSkip += lineSpace;
				center = x;
			}
			else if (*letter == ' ') {
				stbtt_GetBakedQuad(m_font->cdata, m_font->texResolution, m_font->texResolution, *letter - 32, &x, &y, &q, 1);
			} else if (*letter >= 32 && *letter < 128) {
				stbtt_GetBakedQuad(m_font->cdata, m_font->texResolution, m_font->texResolution, *letter - 32, &x, &y, &q, 1);
				
				glm::mat4 model = transform.getTransform()*(*it)->get<Comp::Transform>().getTransform();
				

				float midx = (q.x1-q.x0) / 2.f;
				float midy = (q.y1-q.y0) / 2.f;
				glm::vec4 pos = glm::vec4(-center + q.x0 + midx, lineSkip + q.y0 + midy, 0, 0);
				auto& col = (*it)->get<Comp::Color>();
				LetterVertex vertices[4] = {
					{ col.m_color, (model * glm::vec4(-midx, -midy, 0.f, scale) +pos) / scale, {q.s0, q.t0} },
					{ col.m_color, (model * glm::vec4(midx, -midy, 0.f, scale) + pos) / scale, {q.s1, q.t0} },
					{ col.m_color, (model * glm::vec4(midx,  midy, 0.f, scale) + pos) / scale, {q.s1, q.t1} },
					{ col.m_color, (model * glm::vec4(-midx,  midy, 0.f,scale) + pos) / scale, {q.s0, q.t1} }
				};

				VertexBuffer vb(4 * sizeof(LetterVertex), (void*)vertices);

				VertexArray va;
				va.addVertexBuffer(vb, VertexArrayLayout({
					4, // color
					3, // position
					2, // texCoord
					}));
				m_shader->bind();
				m_shader->setMat4("view", m_camera->getView());
				m_shader->setMat4("proj", m_camera->getProjection());
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_font->ftex);
				m_shader->setInt("uTexture", 0);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
				it++;
			}
			else {
				stbtt_GetBakedQuad(m_font->cdata, m_font->texResolution, m_font->texResolution, 127 - 32, &x, &y, &q, 1);
				//createQuad(q, glm::vec2(-center / scale, lineSkip / scale), i++);
			}
			letter++;
		}
	}

	void Renderer::drawPoint(Comp::Transform& transform, Comp::Color& color, Comp::Point& point)
	{
		Comp::Point::totalVertexCount;
		m_pointData.verticesList[m_pointData.vertexOffset] = { color.m_color,transform.getPosition(), point.m_thickness };
		m_pointData.vertexOffset++;
	}

	void Seden::Renderer::drawShaderQuad(Comp::Transform& transform, Comp::Shader& shader)
	{
		auto sh = shader.getShader();
		sh->bind();
		glm::mat4 model = transform.getTransform();

		glm::vec3 vertices[4] = {
			model * glm::vec4(-1,1,0,1),
			model * glm::vec4(1,1,0,1),
			model * glm::vec4(1,-1,0,1),
			model * glm::vec4(-1,-1,0,1) 
		};

		VertexBuffer vb(4 * sizeof(glm::vec3), (void*)vertices);

		VertexArray va;
		va.addVertexBuffer(vb, VertexArrayLayout({
			3, // position
			}));
		//sh->setMat4("view", m_camera->getView());
		//sh->setMat4("proj", m_camera->getProjection());
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	}
}
