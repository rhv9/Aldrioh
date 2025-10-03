#include <pch.h>
#include "TexturedTiles.h"

#include <Game/RenderDepth.h>
#include <Game/SpriteCollection.h>

#include <Graphics/RenderQueue.h>

TexturedTiles::TexturedTiles(const int spriteId) : spriteId(spriteId)
{
}

TexturedTiles::~TexturedTiles()
{
}

void TexturedTiles::OnUpdate(Timestep ts, const TileMetaData& metadata)
{
}

void TexturedTiles::OnRender(Timestep ts, const TileMetaData& metadata)
{
	glm::vec3 renderPos = { metadata.pos.x * 1.0f, metadata.pos.y * 1.0f, RenderDepth::TILE };
	RenderQueue::EnQueue(RenderLayer::ZERO, renderPos, spriteId);
}
