#include <pch.h>
#include "CollectableChunk.h"
#include <Scene/Scene.h>


void CollectableChunk::Clear()
{
	for (CollectableCell& cell : grid)
		cell.count = 0;
}

void CollectableCell::AddCollectable(const glm::vec2& untrimmedPos, CollectableType type)
{
	uint8_t offsetX = (untrimmedPos.x - (int)untrimmedPos.x) * CCellData::MAX_POINT_VALUE;
	uint8_t offsetY = (untrimmedPos.y - (int)untrimmedPos.y) * CCellData::MAX_POINT_VALUE;
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

glm::vec2 CCellData::GetFloatPos() const
{
	return glm::vec2{ (float)x / CCellData::MAX_POINT_VALUE, (float)y / CCellData::MAX_POINT_VALUE };
}

void CCellData::SetPosOnDecimalValue(const glm::vec2 pos)
{
	x = (pos.x - (int)pos.x) * CCellData::MAX_POINT_VALUE;
	y = (pos.y - (int)pos.y) * CCellData::MAX_POINT_VALUE;
}
