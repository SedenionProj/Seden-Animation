#include <glad/glad.h>

#include "src/logger.h"
#include "src/opengl/shader.hpp"

namespace Seden {
	void Shader::createShader(const char* vShaderCode, const char* fShaderCode, const char* gShaderCode) {
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

	void Shader::link() {
		glLinkProgram(m_id);
		checkCompileErrors(m_id, "PROGRAM");
	}


	void Shader::Bind() const {
		glUseProgram(m_id);
	}
	void Shader::Unind() const {
		glUseProgram(0);
	}

	void Shader::setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value);
	};
	void Shader::setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
	};
	void Shader::setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
	};
	void Shader::setVec3(const std::string& name, const glm::vec3 values) const {
		glUniform3f(glGetUniformLocation(m_id, name.c_str()), values.x, values.y, values.z);
	};
	void Shader::setVec2(const std::string& name, const glm::vec2 values) const {
		glUniform2f(glGetUniformLocation(m_id, name.c_str()), values.x, values.y);
	};
	void Shader::setMat4(const std::string& name, const glm::mat4& matrix, const float instance) const {
		glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	};

	uint32_t Shader::attachShader(const char* shaderCode, uint32_t shaderType, const char* name) {
		GLuint sh = glCreateShader(shaderType);
		glShaderSource(sh, 1, &shaderCode, NULL);
		glCompileShader(sh);
		checkCompileErrors(sh, name);
		glAttachShader(m_id, sh);
		glDeleteShader(sh);
		return sh;
	}

	void Shader::checkCompileErrors(uint32_t shader, std::string type)
	{
		GLint success;
		GLchar infoLog[1024];
		if (type != "PROGRAM") {
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				DEBUG_ERROR("SHADER_COMPILATION_ERROR of type : %s \n%s----------------------------", type.c_str(), infoLog);
			}
		}
		else {
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				DEBUG_ERROR("SHADER_COMPILATION_ERROR of type : %s \n%s----------------------------", type.c_str(), infoLog);
			}
		}
	}
}