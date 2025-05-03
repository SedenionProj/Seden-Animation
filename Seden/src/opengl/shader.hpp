#pragma once

// screen
inline const char* screenFragmentShader = R"(#version 330 core
out vec4 _outFragColor;
uniform int iFrame;
uniform sampler2D uTexture;
in vec3 iFragPos;
void main(){
	_outFragColor = vec4(texture(uTexture, (iFragPos.xy+1.)/2.).rgb,1);
})";

inline const char* screenVertexShader = R"(#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 iFragPos;

void main(){
	iFragPos = aPos;
	gl_Position = vec4(aPos, 1.0);
})";

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

uniform mat4 view;
uniform mat4 proj;

void main(){
	iFragPos = aPos;
	iCol = aCol;
	gl_Position = proj*view*vec4(aPos, 1.0);
})";

inline const char* letterFragmentShader = R"(#version 330 core
out vec4 _outFragColor;
uniform int iFrame;
uniform sampler2D uTexture;
in vec4 iColor;
in vec3 iFragPos;
in vec2 iTexCoords;
void main(){
	float tex = texture(uTexture, iTexCoords).r;
	//if(tex<0.2||iColor.a<0.05) discard;
	_outFragColor = vec4(iColor)*tex;
})";

inline const char* letterVertexShader = R"(#version 330 core
layout (location = 0) in vec4 aColor;
layout (location = 1) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;

out vec4 iColor;
out vec3 iFragPos;
out vec2 iTexCoords;

uniform mat4 view;
uniform mat4 proj;


void main(){
	iColor = aColor;
	iFragPos = aPos;
	iTexCoords = aTexCoords;
	gl_Position = proj*view*vec4(aPos, 1.0);
})";

inline const char* pointFragmentShader = R"(#version 330 core
out vec4 _outFragColor;
in vec4 iColor;

void main(){
	_outFragColor = iColor;
})";

inline const char* pointVertexShader = R"(#version 330 core
layout (location = 0) in vec4 aColor;
layout (location = 1) in vec3 aPos;
layout (location = 2) in float aThickness;

out vec4 iColor;
out vec3 iFragPos;

uniform mat4 view;
uniform mat4 proj;


void main(){
	iColor = aColor;
	iFragPos = aPos;
	gl_Position = proj*view*vec4(aPos, 1.0);
	gl_PointSize = aThickness;
})";

namespace Seden {
	class Shader {
	public:
		Shader() = default;
		Shader(const char* vShaderCode, const char* fShaderCode, const char* gShaderCode = nullptr);
		void createShader(const char* vShaderCode, const char* fShaderCode, const char* gShaderCode = nullptr);
		void createShaderPath(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);

		void link();

		void bind() const;
		void unbind() const;

		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setVec4(const std::string& name, const glm::vec4& values) const;
		void setVec3(const std::string& name, const glm::vec3& values) const;
		void setVec2(const std::string& name, const glm::vec2& values) const;
		void setMat4(const std::string& name, const glm::mat4& matrix, const float instance = 1) const;

		std::filesystem::path m_vertexPath;
		std::filesystem::path m_fragmentPath;
	private:
		uint32_t attachShader(const char* shaderCode, uint32_t shaderType, const char* name);
		std::string processShader(const std::filesystem::path& shaderPath);

		void checkCompileErrors(uint32_t shader, std::string type);

		const char* m_vertexSource;
		const char* m_fragmentSource;
		const char* m_geometrySource;

		uint32_t vertex, fragment, geometry;
		uint32_t m_id;
	};
}