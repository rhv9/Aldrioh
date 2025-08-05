#include <pch.h>
#include "Font.h"

Font::Font(const std::string& fontSheetPath, const glm::vec2& charSize)
{
	fontTexture = std::make_shared<Texture>("assets/textures/free_font/all_16x16.png");
	charSubTextures = new SubTexture[10 * 10];

	for (int i = 0; i < 10 * 10; ++i)
	{
		int x = i % 10;
		int y = i / 10;
		charSubTextures[i] = SubTexture(fontTexture, glm::vec2{ x, y }, { 1, 1 }, charSize);
	}
}

Font::~Font()
{
	delete[] charSubTextures;
}
