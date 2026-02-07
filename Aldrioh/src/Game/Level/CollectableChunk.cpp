#include <pch.h>
#include "CollectableChunk.h"
#include <Scene/Scene.h>

#include <Game/SpriteCollection.h>
#include <Graphics/RenderQueue.h>
#include <Game/RenderDepth.h>

void CollectableChunk::Render(const glm::vec2& offset)
{
	for (int y = 0; y < SIZE; ++y)
	{
		for (int x = 0; x < SIZE; ++x)
		{
			CollectableCell& cell = GetCell(x, y);
			for (int i = 0; i < cell.count; ++i)
			{
				CellItem& cellData = cell.cellArray[i];
				CellItem::RenderData renderData = cellData.GetRenderData();

				glm::vec2 renderPos = glm::vec2{ x, y } + cellData.GetFloatOffset() - renderData.size / 2.0f + offset;

				RenderQueue::EnQueue(RenderLayer::ZERO, glm::vec3{ renderPos, RenderDepth::COLLECTABLES }, renderData.spriteId, renderData.colour, renderData.size);
			}
		}
	}


}

void CollectableChunk::Clear()
{
	for (CollectableCell& cell : grid)
		cell.count = 0;
}

void CollectableCell::AddCollectable(const glm::vec2& untrimmedPos, CollectableType type)
{
	uint8_t offsetX = (untrimmedPos.x - (int)untrimmedPos.x) * CellItem::MAX_POINT_VALUE;
	uint8_t offsetY = (untrimmedPos.y - (int)untrimmedPos.y) * CellItem::MAX_POINT_VALUE;
	cellArray[count++] = { offsetX, offsetY, type };
}

void CollectableCell::AddCollectable(uint8_t xOffset, uint8_t yOffset, CollectableType type)
{
	cellArray[count++] = { xOffset, yOffset, type };
}


std::string CollectableMapping::ToString() const
{
	return std::format("Chunk:({},{}), Cell({},{})", chunkX, chunkY, cellX, cellY);
}

bool CollectableMapping::operator==(const CollectableMapping& other)
{
	return chunkX == other.chunkX && chunkY == other.chunkY && cellX == other.cellX && cellY == other.cellY;
}

glm::vec2 CellItem::GetFloatOffset() const
{
	return glm::vec2{ (float)x / CellItem::MAX_POINT_VALUE, (float)y / CellItem::MAX_POINT_VALUE };
}

void CellItem::SetPosOnDecimalValue(const glm::vec2 pos)
{
	x = (pos.x - (int)pos.x) * CellItem::MAX_POINT_VALUE;
	y = (pos.y - (int)pos.y) * CellItem::MAX_POINT_VALUE;
}

CellItem::RenderData CellItem::GetRenderData() const
{
	static const glm::vec4 JEWEL_COLOURS_ARRAY[4]{ {0, 1, 0, 1}, {0, 0, 1, 1}, {1, 0, 0, 1}, {1, 1, 1, 1} };
	return RenderData{ type == CollectableType::COIN ? Sprites::coin : Sprites::jewel, JEWEL_COLOURS_ARRAY[static_cast<int>(type)] };
}
