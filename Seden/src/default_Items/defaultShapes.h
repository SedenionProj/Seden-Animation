#pragma once

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#include "src/plugin_api/shape.h"
#include "src/default_Items/defaultEffects.h"
#include "src/api/buffer.h"

namespace Seden {

	class Quad : public Shape {
	public:
		Quad(int id) {
			setName(std::to_string(id));
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
		Text(int id) {
			setName(std::to_string(id));
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
			// Load font data once
			unsigned char* ttf_buffer = (unsigned char*)malloc(1 << 20);
			unsigned char temp_bitmap[512 * 512];

			FILE* fontFile = fopen(fontPath.string().c_str(), "rb");
			if (!fontFile) {
				DEBUG_MSG("Error: Failed to open font file.");
				return;
			}
			fread(ttf_buffer, 1, 1 << 20, fontFile);
			fclose(fontFile);

			stbtt_BakeFontBitmap(ttf_buffer, 0, 32.0, temp_bitmap, 512, 512, 32, 96, cdata);
			free(ttf_buffer);

			glGenTextures(1, &ftex);
			glBindTexture(GL_TEXTURE_2D, ftex);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512, 512, 0, GL_RED, GL_UNSIGNED_BYTE, temp_bitmap);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		void draw() override {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ftex);
			shader.setInt("uTexture", 0);

			glBindVertexArray(quadVAO);
			quadIBO.bind();
			glDrawElementsInstanced(GL_TRIANGLES,quadIBO.getCount(), GL_UNSIGNED_INT,0, instanceCount);
		}

		void drawGui() override {
			ImGui::Text("Text Window");
			if (ImGui::InputText("Text", &text)) {
				createText(0.0f, 0.0f, text.data());
			}

			if (ImGui::BeginMenu("Font")) {
				loadFontOptions();
				ImGui::EndMenu();
			}
		}

		void createQuad(const stbtt_aligned_quad& q, uint32_t index) {
			Vertex v[4];
			v[0].pos =		 { q.x0, q.y0 };
			v[0].texCoords = { q.s0, q.t0 };

			v[1].pos =       { q.x1, q.y0 };
			v[1].texCoords = { q.s1, q.t0 };

			v[2].pos =		 { q.x1, q.y1 };
			v[2].texCoords = { q.s1, q.t1 };

			v[3].pos =		 { q.x0, q.y1 };
			v[3].texCoords = { q.s0, q.t1 };

			quadVBO.changeData(4 * sizeof(Vertex), index * 4 * sizeof(Vertex),(void*)v);
		}

		void createText(float x, float y, char* letter) {
			reloadText();
			quadVBO.setData(text.length() * 4 * sizeof(Vertex));
			quadIBO.setQuadLayout(text.length());
			uint32_t i = 0;
			while (*letter) {
				if (*letter >= 32 && *letter < 128) {
					stbtt_aligned_quad q;
					stbtt_GetBakedQuad(cdata, 512, 512, *letter - 32, &x, &y, &q, 1);
					createQuad(q, i++);
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

		stbtt_bakedchar cdata[96]; // ASCII 32..126
		GLuint ftex = 0;
		VertexBuffer quadVBO;
		IndexBuffer quadIBO;
		GLuint quadVAO = 0;
	};

}