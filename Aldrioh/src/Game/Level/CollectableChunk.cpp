#include <pch.h>
#include "CollectableChunk.h"
#include <Scene/Scene.h>


void CollectableChunk::Clear()
{
	for (CollectableCell& cell : grid)
		cell.count = 0;
}

void CollectableCell::AddCollectable(uint8_t x, uint8_t y, CollectableType type)
{
	cellArray[count++] = { x, y, type };
}


std::string CollectableMapping::ToString() const
{
	return std::format("Chunk:({},{}), Cell({},{})", chunkX, chunkY, cellX, cellY);
}

bool CollectableMapping::operator==(const CollectableMapping& other)
{
	return chunkX == other.chunkX && chunkY == other.chunkY && cellX == other.cellX && cellY == other.cellY;
}

