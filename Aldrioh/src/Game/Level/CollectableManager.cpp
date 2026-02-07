#include <pch.h>
#include "CollectableManager.h"

#include <Math/Math.h>

void CollectableManager::Init(int numOfChunkWidth, int numOfChunkHeight)
{
	width = numOfChunkWidth;
	height = numOfChunkHeight;

	chunks.reserve(width * height);

	for (int i = 0; i < width * height; ++i)
		chunks.emplace_back(scene);
}

CollectableMapping CollectableManager::GetMapping(const glm::vec2& pos)
{
	float chunkSize = chunks[0].SIZE;
	glm::vec2 dividedPos = pos / chunkSize;
	CollectableMapping mapping;
	mapping.chunkX = static_cast<int>(dividedPos.x);
	mapping.chunkY = static_cast<int>(dividedPos.y);
	mapping.cellX = static_cast<int>(Math::floatMod(pos.x, chunkSize));
	mapping.cellY = static_cast<int>(Math::floatMod(pos.y, chunkSize));

	return mapping;
}

void CollectableManager::RenderChunks(CollectableMapping& bottomLeft, CollectableMapping& topRight)
{
	for (int y = bottomLeft.chunkY; y < topRight.chunkY; ++y)
	{
		for (int x = bottomLeft.chunkX; x < topRight.chunkX; ++x)
		{
			CollectableChunk& chunk = GetChunk(x, y);
			chunk.Render({ x * chunk.SIZE , y * chunk.SIZE });
		}
	}
}
