#pragma once
#include <stb_truetype.h>
#include <filesystem>
#include <glad/glad.h>

namespace Seden {
	class Font{
	public:
		Font();
		Font(const std::filesystem::path& path);

		void reloadFont(const std::filesystem::path& fontPath);

	public:
		stbtt_bakedchar cdata[96]; // ASCII 32..126
		GLuint ftex = 0; // todo: abstract OpenGL texture
		int texResolution = 2046;
		int fontSize = 64;
	};
}