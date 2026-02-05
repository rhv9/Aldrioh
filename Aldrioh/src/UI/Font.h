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
	SubTexture* GetCharSubTexture(const char c);

	SubTexture* GetBlockSubTexture();
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

	FontStyle& WithSize(float size) { this->size = size; return *this; }
	FontStyle& WithCharSpacingPercent(float charSpacingPercent) { this->charSpacingPercent = charSpacingPercent; return *this; }
	FontStyle& WithColour(const glm::vec4& colour) { this->colour = colour; return *this; }

	float CalculateTextWidth(const std::string& text) const;
};