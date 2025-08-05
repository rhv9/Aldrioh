#pragma once

#include <Graphics/SubTexture.h>

class Font
{
public:
	Font(const std::string& fontSheetPath, const glm::vec2& charSize);
	~Font();
	


private:
	std::shared_ptr<Texture> fontTexture;
	SubTexture* charSubTextures;
};