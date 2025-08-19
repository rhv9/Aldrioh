#pragma once

#include <Graphics/SubTexture.h>
#include "Colour.h"

class Font
{
public:
	static inline Font* DEFAULT = nullptr;

	static void InitGlobalFonts();
	static void DestroyGlobalFonts();
public:
	Font(const std::string& fontSheetPath, const glm::vec2& charSize);
	~Font();
	
	const std::shared_ptr<Texture>& GetTexture() { return fontTexture; }
	const SubTexture* GetCharSubTexture(const char c);

	const SubTexture* GetBlockSubTexture();
private:
	std::shared_ptr<Texture> fontTexture;
	SubTexture* charSubTextures;
};

struct FontStyle
{
	Font* font = Font::DEFAULT;
	float size = 10.0f;
	float charSpacingPercent = 0.8f;
	glm::vec4 colour = Colour::BLACK;

	float CalculateTextWidth(const std::string& text);
};