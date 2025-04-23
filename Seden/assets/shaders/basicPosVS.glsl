#version 460

layout (location = 0) in vec2 aPos;

out vec2 iFragPos;

void main(){
	iFragPos = aPos;
	gl_Position = vec4(aPos, 0.0, 1.0);
}