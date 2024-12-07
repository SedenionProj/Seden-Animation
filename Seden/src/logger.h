#pragma once
#include <iostream>

#define DEBUG_ERROR(Message,...) printf("ERROR: " Message "\n", ##__VA_ARGS__)
#define DEBUG_MSG(Message,...) printf("MESSAGE: " Message "\n", ##__VA_ARGS__)
#define DEBUG_WARNING(Message,...) printf("WARNING: " Message "\n", ##__VA_ARGS__)
#define DEBUG_CHECK(x,...) if(!x) DEBUG_WARNING(__VA_ARGS__)
#define DEBUG_CHECK_OPENGL() GLenum error = glGetError();\
							if (error != GL_NO_ERROR) {\
								std::cerr << "OpenGL Error: " << error << std::endl;\
							}