#pragma once

#include <Graphics/SubTexture.h>

class Font
{
public:
	Font(const std::string& fontSheetPath, const glm::vec2& charSize);
	~Font();
	
	const std::shared_ptr<Texture>& GetTexture() { return fontTexture; }
	const SubTexture* GetCharSubTexture(const char c);

private:
	std::shared_ptr<Texture> fontTexture;
	SubTexture* charSubTextures;
};