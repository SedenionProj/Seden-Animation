#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

//tmp
inline const char* baseFragmentShader = R"(#version 330 core
out vec4 _outFragColor;
uniform int iFrame;
in vec3 iFragPos;
in vec3 iCol;
void main(){
	_outFragColor = vec4(vec3(iCol),1);
})";

inline const char* baseVertexShader = R"(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

out vec3 iFragPos;
out vec3 iCol;

void main(){
	iFragPos = aPos;
	iCol = aCol;
	gl_Position = vec4(aPos, 1.0);
})";

namespace Seden {
	class Shader {
	public:
		void createShader(const char* vShaderCode, const char* fShaderCode, const char* gShaderCode = nullptr);
		void link();


		void Bind() const;
		void Unind() const;

		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setVec3(const std::string& name, const glm::vec3 values) const;
		void setVec2(const std::string& name, const glm::vec2 values) const;
		void setMat4(const std::string& name, const glm::mat4& matrix, const float instance = 1) const;
	private:
		uint32_t attachShader(const char* shaderCode, uint32_t shaderType, const char* name);

		void checkCompileErrors(uint32_t shader, std::string type);

		const char* m_vertexSource;
		const char* m_fragmentSource;
		const char* m_geometrySource;

		uint32_t vertex, fragment, geometry;
		uint32_t m_id;
	};
}