#pragma once
#include <stdio.h>

#if 1
	#if defined(SEDEN_VERBOSE)
		#define DEBUG_ERROR(Message, ...) \
		    printf("ERROR: " Message " [%s:%d]\n", __FILE__, __LINE__, ##__VA_ARGS__)
		#define DEBUG_MSG(Message, ...) \
		    printf("MSG: " Message " [%s:%d]\n", __FILE__, __LINE__, ##__VA_ARGS__)
		#define DEBUG_WARNING(Message, ...) \
		    printf("WARNING: " Message " [%s:%d]\n", __FILE__, __LINE__, ##__VA_ARGS__)
	#else
		#define DEBUG_ERROR(Message,...) printf("ERROR: " Message "\n", ##__VA_ARGS__)
		#define DEBUG_MSG(Message,...) printf("MESSAGE: " Message "\n", ##__VA_ARGS__)
		#define DEBUG_WARNING(Message,...) printf("WARNING: " Message "\n", ##__VA_ARGS__)
	#endif

	#define DEBUG_ASSERT(x, ...) \
	    if (!(x)) DEBUG_WARNING("Assertion failed: " __VA_ARGS__)
	#define DEBUG_CHECK_OPENGL() switch (glGetError()){\
			case GL_INVALID_ENUM:\
				DEBUG_ERROR("GL_INVALID_ENUM");\
				break;\
			case GL_INVALID_VALUE:\
				DEBUG_ERROR("GL_INVALID_VALUE");\
				break;\
			case GL_INVALID_OPERATION:\
				DEBUG_ERROR("GL_INVALID_OPERATION");\
				break;\
			case GL_STACK_OVERFLOW:\
				DEBUG_ERROR("GL_STACK_OVERFLOW");\
				break;\
			case GL_STACK_UNDERFLOW:\
				DEBUG_ERROR("GL_STACK_UNDERFLOW");\
				break;\
			case GL_OUT_OF_MEMORY:\
				DEBUG_ERROR("GL_OUT_OF_MEMORY");\
				break;\
			case GL_INVALID_FRAMEBUFFER_OPERATION:\
				DEBUG_ERROR("GL_INVALID_FRAMEBUFFER_OPERATION");\
				break;\
			case GL_CONTEXT_LOST:\
				DEBUG_ERROR("GL_CONTEXT_LOST");\
				break;\
			default:\
				break;\
		}
#else
	#define DEBUG_ERROR(Message, ...)
	#define DEBUG_MSG(Message, ...)
	#define DEBUG_WARNING(Message, ...)
	#define DEBUG_ASSERT(x, ...)
	#define DEBUG_CHECK_OPENGL()
#endif