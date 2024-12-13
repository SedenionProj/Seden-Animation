#pragma once

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#include "src/plugin_api/shape.h"
#include "src/default_Items/defaultEffects.h"
#include "src/api/buffer.h"
#include "src/scene.h"

namespace Seden {

	class Quad : public Shape {
	public:
		Quad(std::shared_ptr<Scene> scene) : Shape(scene) {
			setName("Quad");
			shader.createShader(baseVertexShader, baseFragmentShader.c_str());

			addEffect(std::make_shared<Transform>());
			addEffect(std::make_shared<SolidColorEff>());
			addEffect(std::make_shared<Instance>());

			createQuad();
		}

		~Quad() {
			glDeleteVertexArrays(1, &quadVAO);
		}

		void draw() override {
			glBindVertexArray(quadVAO);
			glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, instanceCount);
		}

		void drawGui() override {}

		void createQuad() {
			float vertices[]{
				-1.f, -1.f,
				-1.f,  1.f,
				 1.f,  1.f,
				 1.f, -1.f
			};

			glGenVertexArrays(1, &quadVAO);
			glBindVertexArray(quadVAO);

			quadVBO.setData(sizeof(vertices), vertices);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glBindVertexArray(0);
		}
	private:
		VertexBuffer quadVBO;
		GLuint quadVAO;
	};

	inline std::string baseFragmentShaderText = R"(#version 330 core
out vec4 _outFragColor;
uniform int iFrame;
in vec2 iFragPos;
in vec2 iTexCoords;
uniform sampler2D uTexture;
//[code]
void main(){
	_outFragColor = vec4(0);
//[apply]
	_outFragColor *= vec4(vec3(texture(uTexture, iTexCoords).r),1);
})";

	inline const char* baseVertexShaderText = R"(#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 iProjection;
uniform mat4 iModel;

out vec2 iFragPos;
out vec2 iTexCoords;

uniform int iFrame;

void main(){
	iFragPos = aPos;
	iTexCoords = aTexCoords;
//[code]
})";

	class Text : public Shape {
	public:
		Text(std::shared_ptr<Scene> scene) : Shape(scene) {
			setName("Text");
			shader.createShader(baseVertexShaderText, baseFragmentShaderText.c_str());

			addEffect(std::make_shared<Transform>());
			addEffect(std::make_shared<SolidColorEff>());
			addEffect(std::make_shared<Instance>());

			glGenVertexArrays(1, &quadVAO);
			glBindVertexArray(quadVAO);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
			glEnableVertexAttribArray(1);

			glBindVertexArray(0);

			createText(0.0f, 0.0f, text.data());
		}

		void reloadText() {

			unsigned char* ttf_buffer = (unsigned char*)malloc(1 << 20);
			unsigned char* temp_bitmap = (unsigned char*)malloc(texResolution * texResolution);

			FILE* fontFile = fopen(fontPath.string().c_str(), "rb");
			if (!fontFile) {
				DEBUG_MSG("Error: Failed to open font file.");
				return;
			}
			fread(ttf_buffer, 1, 1 << 20, fontFile);
			fclose(fontFile);

			stbtt_BakeFontBitmap(ttf_buffer, 0, 64, temp_bitmap, texResolution, texResolution, 32, 96, cdata);
			free(ttf_buffer);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			glGenTextures(1, &ftex);
			glBindTexture(GL_TEXTURE_2D, ftex);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, texResolution, texResolution, 0, GL_RED, GL_UNSIGNED_BYTE, temp_bitmap);
			free(temp_bitmap);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		void draw() override {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ftex);
			shader.setInt("uTexture", 0);

			glBindVertexArray(quadVAO);
			quadIBO.bind();
			glDrawElementsInstanced(GL_TRIANGLES, quadIBO.getCount(), GL_UNSIGNED_INT, 0, instanceCount);
		}

		void drawGui() override {
			ImGui::Text("Text Window");
			ImVec2 wsize = ImGui::GetWindowSize();
			wsize.x -= 10;
			if (ImGui::DragFloat("line space", &lineSpace)) {
				createText(0.0f, 0.0f, text.data());
			}

			if (ImGui::DragInt("Resolution", &texResolution, 1,0, 2048)) {
				createText(0.0f, 0.0f, text.data());
			}

			if (ImGui::InputTextMultiline("##", &text, wsize)) {
				createText(0.0f, 0.0f, text.data());
			}

			if (ImGui::BeginMenu("Font")) {
				loadFontOptions();
				ImGui::EndMenu();
			}
		}

		void createQuad(const stbtt_aligned_quad& q, glm::vec2 pos, uint32_t index) {
			Vertex v[4];

			v[0].pos = glm::vec2(q.x0, q.y0) / scale + pos;
			v[0].texCoords = glm::vec2(q.s0, q.t0);

			v[1].pos = glm::vec2(q.x1, q.y0) / scale + pos;
			v[1].texCoords = glm::vec2(q.s1, q.t0);

			v[2].pos = glm::vec2(q.x1, q.y1) / scale + pos;
			v[2].texCoords = glm::vec2(q.s1, q.t1);

			v[3].pos = glm::vec2(q.x0, q.y1) / scale + pos;
			v[3].texCoords = glm::vec2(q.s0, q.t1);

			//DEBUG_MSG("%f %f %f %f", q.x0, q.x1, q.y0, q.y1);

			quadVBO.changeData(4 * sizeof(Vertex), index * 4 * sizeof(Vertex), (void*)v);
		}

		void createText(float x, float y, char* letter) {
			reloadText();

			quadVBO.setData(text.length() * 4 * sizeof(Vertex));
			quadIBO.setQuadLayout(text.length());
			uint32_t i = 0;
			float lineSkip = 0;
			float center = 0;
			stbtt_aligned_quad q;
			while (*letter) {
				if (*letter == '\n') {
					lineSkip += lineSpace;
					center = x;

					char zeroData[4 * sizeof(Vertex)] = { 0 };
					quadVBO.changeData(4 * sizeof(Vertex), i++ * 4 * sizeof(Vertex), (void*)zeroData);

				} else if (*letter >= 32 && *letter < 128) {
					stbtt_GetBakedQuad(cdata, texResolution, texResolution, *letter - 32, &x, &y, &q, 1);
					createQuad(q, glm::vec2(-center / scale, lineSkip / scale), i++);
				} else {
					stbtt_GetBakedQuad(cdata, texResolution, texResolution, 127-32, &x, &y, &q, 1);
					createQuad(q, glm::vec2(-center / scale, lineSkip / scale), i++);
				}
				letter++;
			}
		}

	private:
		void loadFontOptions() {
			char* userpath = getenv("USERPROFILE");

			for (const auto& entry : fs::directory_iterator("c:/windows/fonts")) {
				std::string currFont = entry.path().filename().string();
				if (ImGui::MenuItem(currFont.c_str())) {
					fontPath = entry.path();
					createText(0.0f, 0.0f, text.data());
				}
			}

			std::filesystem::path userFontsPath = std::filesystem::path(userpath) / "AppData/Local/Microsoft/Windows/Fonts";
			for (const auto& entry : fs::directory_iterator(userFontsPath)) {
				std::string currFont = entry.path().filename().string();
				if (ImGui::MenuItem(currFont.c_str())) {
					fontPath = entry.path();
					createText(0.0f, 0.0f, text.data());
				}
			}
		}

		struct Vertex {
			glm::vec2 pos;
			glm::vec2 texCoords;
		};

		std::string text = "Text";
		std::filesystem::path fontPath = "c:/windows/fonts/times.ttf";
		float scale = 100;
		float lineSpace = 30;
		int texResolution = 2046;

		stbtt_bakedchar cdata[96]; // ASCII 32..126
		GLuint ftex = 0;
		VertexBuffer quadVBO;
		IndexBuffer quadIBO;
		GLuint quadVAO = 0;
	};

}