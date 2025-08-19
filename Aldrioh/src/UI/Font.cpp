#include <pch.h>
#include "Font.h"

static constexpr int SHEET_LENGTH = 10;

void Font::InitGlobalFonts()
{
	DEFAULT = new Font("assets/textures/free_font/all_16x16.png", { 16, 16 });
}

void Font::DestroyGlobalFonts()
{
	delete Font::DEFAULT;
}

Font::Font(const std::string& fontSheetPath, const glm::vec2& charSize)
{
	fontTexture = std::make_shared<Texture>(fontSheetPath);
	charSubTextures = new SubTexture[SHEET_LENGTH * SHEET_LENGTH];

	for (int i = 0; i < SHEET_LENGTH * SHEET_LENGTH; ++i)
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
	int ascii = int(c);
	if (c == ' ')
		return &charSubTextures[0];
	if (ascii < int('!') || ascii > int('~'))
		return &charSubTextures[SHEET_LENGTH * SHEET_LENGTH - 1];

	// + 1 as first subtexture is blank on png
	return &charSubTextures[ascii - int('!') + 1];
}

const SubTexture* Font::GetBlockSubTexture()
{
	return &charSubTextures['c'];
}




// FONT STYLE

float FontStyle::CalculateTextWidth(const std::string& text)
{
	return text.size() * size * charSpacingPercent;
}
