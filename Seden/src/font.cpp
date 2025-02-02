#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>
#include <glad/glad.h>
#include <memory>
#include "src/font.hpp"
#include "src/logger.h"

namespace Seden {
	
	Font::Font(const std::filesystem::path& fontPath)
	{
		reloadFont(fontPath);
	}

	Font::Font() : Font("c:/windows/fonts/times.ttf") {}

	void Font::reloadFont(const std::filesystem::path& fontPath) {

		unsigned char* ttf_buffer = (unsigned char*)malloc(1 << 20);
		unsigned char* temp_bitmap = (unsigned char*)malloc(texResolution * texResolution);

		FILE* fontFile = fopen(fontPath.string().c_str(), "rb");
		if (!fontFile) {
			DEBUG_MSG("Error: Failed to open font file.");
			return;
		}
		fread(ttf_buffer, 1, 1 << 20, fontFile);
		fclose(fontFile);

		stbtt_BakeFontBitmap(ttf_buffer, 0, 64, temp_bitmap, texResolution, texResolution, 32, 96, cdata);
		free(ttf_buffer);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glGenTextures(1, &ftex);
		glBindTexture(GL_TEXTURE_2D, ftex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, texResolution, texResolution, 0, GL_RED, GL_UNSIGNED_BYTE, temp_bitmap);
		free(temp_bitmap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
}