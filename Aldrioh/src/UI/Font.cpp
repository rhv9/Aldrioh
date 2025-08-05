#include <pch.h>
#include "Font.h"

Font::Font(const std::string& fontSheetPath, const glm::vec2& charSize)
{
	fontTexture = std::make_shared<Texture>(fontSheetPath);
	charSubTextures = new SubTexture[10 * 10];

	for (int i = 0; i < 10 * 10; ++i)
	{
		int x = i % 10;
		int y = 9 - (i / 10);
		charSubTextures[i] = SubTexture(fontTexture, glm::vec2{ x, y }, { 16.0f, 16.0f });
	}
}

Font::~Font()
{
	delete[] charSubTextures;
}

const SubTexture* Font::GetCharSubTexture(const char c)
{
	LOG_CORE_INFO("{}", int('!'));
	int ascii = int(c);
	ASSERT(ascii >= int('!') && ascii <= int('~'), "Cannot render this char!");
	// + 1 as first subtexture is blank on png
	return &charSubTextures[ascii - int('!') + 1];
}
