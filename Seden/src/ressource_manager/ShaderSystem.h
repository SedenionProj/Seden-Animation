#pragma once
#include <glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "src/logger.h"
#include "src/plugin_api/effect.h"
#include "src/ressource_manager/fileSystem.h"

/*
input variables:

iResolution
iFragPos
iFrame;
iProjection
iModel

gl_FragCoord


*/

//tmp
inline std::string baseFragmentShader = R"(#version 330 core
out vec4 _outFragColor;
uniform int iFrame;
in vec2 iFragPos;
//[code]
void main(){
	_outFragColor = vec4(0);
//[apply]
})";

inline const char* baseVertexShader = R"(#version 330 core
layout (location = 0) in vec2 aPos;

uniform mat4 iProjection;
uniform mat4 iModel;

out vec2 iFragPos;

uniform int iFrame;

void main(){
	iFragPos = aPos;
//[code]
})";

class ShaderSystem {
public:
	void createShader(const char* vShaderCode, const char* fShaderCode, const char* gShaderCode = nullptr) {
		m_id = glCreateProgram();

		vertex = attachShader(vShaderCode, GL_VERTEX_SHADER, "VERTEX");
		fragment = attachShader(fShaderCode, GL_FRAGMENT_SHADER, "FRAGMENT");
		if (gShaderCode)
			geometry = attachShader(gShaderCode, GL_GEOMETRY_SHADER, "GEOMETRY");

		link();

		m_vertexSource = vShaderCode;
		m_fragmentSource = fShaderCode;
		m_geometrySource = gShaderCode;
	}
	void reAttachShader(const char* shaderCode, GLenum shaderType, const char* name) {
		GLuint* shaderPtr = nullptr;
		switch (shaderType)
		{
		case GL_VERTEX_SHADER:
			shaderPtr = &vertex;
			break;
		case GL_FRAGMENT_SHADER:
			shaderPtr = &fragment;
			break;
		case GL_GEOMETRY_SHADER:
			shaderPtr = &geometry;
			break;
		}

		glDetachShader(m_id, *shaderPtr);
		*shaderPtr = attachShader(shaderCode, shaderType, name);
	}
	void link() {
		glLinkProgram(m_id);
		checkCompileErrors(m_id, "PROGRAM");
	}

	void reconstructShader(std::vector<std::shared_ptr<Effect>> effects) {
		FileSystem baseFS(m_fragmentSource, FileSystem::Type::STRING_FILE);
		
		FileSystem codes;
		FileSystem mixEffects;

		for (std::shared_ptr<Effect> effect : effects) {
			std::string shaderCode = effect->getShaderCode();
			if(shaderCode == "")
				continue;
			if (effect->type == Effect::Type::VERTEX) {
				FileSystem baseVS(m_vertexSource, FileSystem::Type::STRING_FILE);
				baseVS.addAfter("//[code]", "\n"+shaderCode);
				reAttachShader(baseVS.getFile().c_str(), GL_VERTEX_SHADER, "new vertex");
				continue;
			}
			std::string token = "+="; // todo custom token
			FileSystem effectShader(shaderCode, FileSystem::Type::STRING_FILE);

			std::string entryPoint = "_main" + effect->getName();
			effectShader.replace("_main", entryPoint);
			std::string mix = "_outFragColor"+ token + entryPoint+"(_outFragColor);\n";

			if(!codes.find(entryPoint))
				codes.add(effectShader.getFile());
			mixEffects.add(mix);
		}

		baseFS.addAfter("//[code]", codes.getFile());
		baseFS.addAfter("//[apply]","\n"+ mixEffects.getFile());
		
		reAttachShader(baseFS.getFile().c_str(), GL_FRAGMENT_SHADER, "new fragment");
		link();
	};

	void Bind() const {
		glUseProgram(m_id);
	}
	void Unind() const {
		glUseProgram(0);
	}

	void setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value);
	};
	void setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
	};
	void setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
	};
	void setVec3(const std::string& name, const glm::vec3 values) const {
		glUniform3f(glGetUniformLocation(m_id, name.c_str()), values.x, values.y, values.z);
	};
	void setVec2(const std::string& name, const glm::vec2 values) const {
		glUniform2f(glGetUniformLocation(m_id, name.c_str()), values.x, values.y);
	};
	void setMat4(const std::string& name, const glm::mat4& matrix, const float instance = 1) const {
		glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	};
private:
	GLuint attachShader(const char* shaderCode, GLenum shaderType, const char* name) {
		GLuint sh = glCreateShader(shaderType);
		glShaderSource(sh, 1, &shaderCode, NULL);
		glCompileShader(sh);
		checkCompileErrors(sh, name);
		glAttachShader(m_id, sh);
		glDeleteShader(sh);
		return sh;
	}

	void checkCompileErrors(GLuint shader, std::string type)
	{
		GLint success;
		GLchar infoLog[1024];
		if (type != "PROGRAM"){
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				DEBUG_ERROR("SHADER_COMPILATION_ERROR of type : %s \n%s----------------------------", type.c_str(), infoLog);
			}
		}
		else{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success){
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				DEBUG_ERROR("SHADER_COMPILATION_ERROR of type : %s \n%s----------------------------", type.c_str(), infoLog);
			}
		}
	}

	const char* m_vertexSource;
	const char* m_fragmentSource;
	const char* m_geometrySource;

	GLuint vertex, fragment, geometry;
	GLuint m_id;
};