#include "pch.h"
#include "SubTexture.h"

SubTexture::SubTexture(const std::shared_ptr<Texture>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize) : textureParent(texture.get())
{

	glm::vec2 pixelCenterOffset{ (1.0f / texture->GetWidth()) / 128.0f,(1.0f / texture->GetHeight()) / 128.0f };

	const glm::vec2 bottomLeftCoords{
		(coords.x * cellSize.x) / texture->GetWidth() + pixelCenterOffset.x,
		(coords.y * cellSize.y) / texture->GetHeight() + pixelCenterOffset.y,
	};

	const glm::vec2 topRightCoords = {
		((coords.x + spriteSize.x) * cellSize.x) / texture->GetWidth() - pixelCenterOffset.x,
		((coords.y + spriteSize.y) * cellSize.y) / texture->GetHeight() - pixelCenterOffset.y
	};

	width = cellSize.x * spriteSize.x;
	height = cellSize.y * spriteSize.y;

	textureCoords = { bottomLeftCoords, topRightCoords };
}

