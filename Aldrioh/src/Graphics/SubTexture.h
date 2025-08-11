#pragma once
#include "Texture.h"

struct SubTexture
{
	SubTexture(const std::shared_ptr<Texture>& texture, const glm::vec2& position, const glm::vec2& cellSize, const glm::vec2& spriteSize = {1.0f, 1.0f});
	SubTexture() = default;

	glm::vec2 GetSize() const { return glm::vec2{ width, height }; }
	
	TextureCoords textureCoords;
	uint32_t width = 0, height = 0;

	Texture* textureParent = nullptr;
};