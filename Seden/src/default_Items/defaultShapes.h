#pragma once

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#include "src/plugin_api/shape.h"
#include "src/default_Items/defaultEffects.h"

class Quad : public Shape {
public:
	Quad(int id) {
		setName(std::to_string(id));
		shader.createShader(baseVertexShader, baseFragmentShader.c_str());
		createQuad();
		addEffect(std::make_shared<Transform>());
		addEffect(std::make_shared<SolidColorEff>());
		addEffect(std::make_shared<Instance>());
	}

	void draw() override {
		glBindVertexArray(quadVBO);
		glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4,instanceCount);
	}

	void createQuad() {
		float vertices[]{
			-1.f, -1.f,
			-1.f,  1.f,
			 1.f,  1.f,
			 1.f, -1.f
		};

		glGenVertexArrays(1, &quadVAO);
		glBindVertexArray(quadVAO);

		glGenBuffers(1, &quadVBO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}
private:
	GLuint quadVBO;
	GLuint quadVAO;
};

inline std::string baseFragmentShaderText = R"(#version 330 core
out vec4 _outFragColor;
uniform int iFrame;
in vec2 iFragPos;
uniform sampler2D uTexture;
//[code]
void main(){
	_outFragColor = vec4(0);
//[apply]
	_outFragColor = vec4(texture(uTexture, (iFragPos+1.)/2.).rgb,1);
})";

inline const char* baseVertexShaderText = R"(#version 330 core
layout (location = 0) in vec2 aPos;

uniform mat4 iProjection;
uniform mat4 iModel;

out vec2 iFragPos;

uniform int iFrame;

void main(){
	iFragPos = aPos;
//[code]
})";

class Text : public Shape {
public:
	Text(int id) {
		setName(std::to_string(id));
		shader.createShader(baseVertexShaderText, baseFragmentShaderText.c_str());
		createQuad();
		addEffect(std::make_shared<Transform>());
		addEffect(std::make_shared<SolidColorEff>());
		addEffect(std::make_shared<Instance>());
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		fread(ttf_buffer, 1, 1 << 20, fopen("c:/windows/fonts/times.ttf", "rb"));
		stbtt_BakeFontBitmap(ttf_buffer, 0, 32.0, temp_bitmap, 512, 512, 32, 96, cdata);
		// can free ttf_buffer at this point
		glGenTextures(1, &ftex);
		glBindTexture(GL_TEXTURE_2D, ftex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512, 512, 0, GL_RED, GL_UNSIGNED_BYTE, temp_bitmap);
		// can free temp_bitmap at this point
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	}

	void draw() override {
		stbtt_aligned_quad q;
		float x =10., y = 10.;
		stbtt_GetBakedQuad(cdata, 512, 512, 1, &x, &y, &q, 1);
		//DEBUG_MSG("%f %f %f", x, y, q.x0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ftex);
		shader.setInt("uTexture", 0);
		glBindVertexArray(quadVBO);
		glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, instanceCount);
	}

	void createQuad() {
		float vertices[]{
			-1.f, -1.f,
			-1.f,  1.f,
			 1.f,  1.f,
			 1.f, -1.f
		};

		glGenVertexArrays(1, &quadVAO);
		glBindVertexArray(quadVAO);

		glGenBuffers(1, &quadVBO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}
private:
	unsigned char ttf_buffer[1 << 20];
	unsigned char temp_bitmap[512 * 512];

	stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
	GLuint ftex;

	GLuint quadVBO;
	GLuint quadVAO;
};