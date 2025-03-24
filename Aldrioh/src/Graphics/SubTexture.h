#pragma once
#include "Texture.h"

class SubTexture : public Texture
{
public:
	SubTexture() = default;

	SubTexture& operator=(const SubTexture& other) = delete;
	SubTexture& operator=(SubTexture&& other);

	SubTexture(const std::shared_ptr<Texture>& texture, const glm::vec2& position, const glm::vec2& cellSize, const glm::vec2& spriteSize = {1.0f, 1.0f});

private:
	std::shared_ptr<Texture> textureParent;
};