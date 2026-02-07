#include <pch.h>
#include "CollectableChunk.h"
#include <Scene/Scene.h>

#include <Game/SpriteCollection.h>
#include <Graphics/RenderQueue.h>
#include <Game/RenderDepth.h>

// Collectable Chunk

CollectableChunk::CollectableChunk(const std::vector<CachedCollectableBlock>& cachedBlocks)
{
	for (const CachedCollectableBlock& cachedBlock : cachedBlocks)
	{
		CollectableBlock& block = GetBlock(cachedBlock.x, cachedBlock.y);
		block.count = cachedBlock.cellVector.size();
		for (int i = 0; i < block.count; ++i)
			block.cellArray[i] = cachedBlock.cellVector[i];
	}
}

void CollectableChunk::Render(const glm::vec2& offset)
{
	for (int y = 0; y < SIZE; ++y)
	{
		for (int x = 0; x < SIZE; ++x)
		{
			CollectableBlock& cell = GetBlock(x, y);
			for (int i = 0; i < cell.count; ++i)
			{
				CollectableItem& cellData = cell.cellArray[i];
				CollectableItem::RenderData renderData = cellData.GetRenderData();

				glm::vec2 renderPos = glm::vec2{ x, y } + cellData.GetFloatOffset() - renderData.size / 2.0f + offset;

				RenderQueue::EnQueue(RenderLayer::ZERO, glm::vec3{ renderPos, RenderDepth::COLLECTABLES }, renderData.spriteId, renderData.colour, renderData.size);
			}
		}
	}
}

void CollectableChunk::Clear()
{
	for (CollectableBlock& cell : grid)
		cell.count = 0;
}


// Cell Mapping

std::string CollectableMapping::ToString() const
{
	return std::format("Chunk:({},{}), Cell({},{})", chunkX, chunkY, cellX, cellY);
}

bool CollectableMapping::operator==(const CollectableMapping& other)
{
	return chunkX == other.chunkX && chunkY == other.chunkY && cellX == other.cellX && cellY == other.cellY;
}


// Cell Item

void CollectableBlock::AddCollectable(const glm::vec2& untrimmedPos, CollectableType type)
{
	uint8_t offsetX = (untrimmedPos.x - (int)untrimmedPos.x) * CollectableItem::MAX_POINT_VALUE;
	uint8_t offsetY = (untrimmedPos.y - (int)untrimmedPos.y) * CollectableItem::MAX_POINT_VALUE;
	cellArray[count++] = { offsetX, offsetY, type };
}

void CollectableBlock::AddCollectable(uint8_t xOffset, uint8_t yOffset, CollectableType type)
{
	cellArray[count++] = { xOffset, yOffset, type };
}

glm::vec2 CollectableItem::GetFloatOffset() const
{
	return glm::vec2{ (float)x / CollectableItem::MAX_POINT_VALUE, (float)y / CollectableItem::MAX_POINT_VALUE };
}

void CollectableItem::SetPosOnDecimalValue(const glm::vec2 pos)
{
	x = (pos.x - (int)pos.x) * CollectableItem::MAX_POINT_VALUE;
	y = (pos.y - (int)pos.y) * CollectableItem::MAX_POINT_VALUE;
}

CollectableItem::RenderData CollectableItem::GetRenderData() const
{
	static const glm::vec4 JEWEL_COLOURS_ARRAY[4]{ {0, 1, 0, 1}, {0, 0, 1, 1}, {1, 0, 0, 1}, {1, 1, 1, 1} };
	return RenderData{ type == CollectableType::COIN ? Sprites::coin : Sprites::jewel, JEWEL_COLOURS_ARRAY[static_cast<int>(type)] };
}
