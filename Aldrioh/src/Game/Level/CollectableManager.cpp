#include <pch.h>
#include "CollectableManager.h"

#include <Math/Math.h>
#include <Game.h>
#include <Game/Level/Level.h>

#include <Graphics/RenderQueue.h>
#include <Game/SpriteCollection.h>
#include <UI/Colour.h>
#include <Game/RenderDepth.h>

CollectableMapping CollectableManager::GetMapping(const glm::vec2& pos)
{
	float chunkSize = CollectableChunk::SIZE;
	glm::vec2 dividedPos = pos / chunkSize;
	CollectableMapping mapping;
	mapping.chunkX = static_cast<int>(dividedPos.x);
	mapping.chunkY = static_cast<int>(dividedPos.y);
	mapping.cellX = static_cast<int>(Math::floatMod(pos.x, chunkSize));
	mapping.cellY = static_cast<int>(Math::floatMod(pos.y, chunkSize));

	return mapping;
}


inline CollectableChunk& CollectableManager::GetChunk(int x, int y)
{
	uint64_t val = static_cast<uint64_t>(x) << 32 | static_cast<uint64_t>(y);

	if (loadedChunks.find(val) != loadedChunks.end())
	{
		CollectableChunk& chunk = loadedChunks[val];
		chunk.ResetTimer();
		return chunk;
	}
	else
	{
		if (unloadedChunks.find(val) != unloadedChunks.end())
		{
			loadedChunks[val] = CollectableChunk{ unloadedChunks[val] };
			unloadedChunks.erase(val);
			return loadedChunks[val];
		}
		else
		{
			loadedChunks.insert({ val, CollectableChunk{} });
			return loadedChunks[val];
		}

	}

}

void CollectableManager::RenderChunks(CollectableMapping& bottomLeft, CollectableMapping& topRight)
{
	for (int y = bottomLeft.chunkY; y < topRight.chunkY + 1; ++y)
		for (int x = bottomLeft.chunkX; x < topRight.chunkX + 1; ++x)
			GetChunk(x, y).Render({ x * CollectableChunk::SIZE , y * CollectableChunk::SIZE });
}

void CollectableManager::OnUpdate(Timestep ts, const CollectableMapping& bottomLeftMapping, const CollectableMapping& topRightMapping)
{
	// Only run every second
	if (Game::Instance().GetTickCount() % Game::Instance().GetTicksTarget() != 0)
		return;

	auto iter = loadedChunks.begin();
	//LOG_INFO("Chunk X, Y ({}-{}, {}-{})", bottomLeftMapping.chunkX, topRightMapping.chunkX, bottomLeftMapping.chunkY, topRightMapping.chunkY);
	for (auto& iter : loadedChunks)
	{
		CollectableChunk& chunk = iter.second;
		
		auto [x, y] = GetXY(iter.first);

		if (x >= bottomLeftMapping.chunkX && x <= topRightMapping.chunkX && y >= bottomLeftMapping.chunkY && y <= topRightMapping.chunkY)
		{
			chunk.ResetTimer();
			continue;
		}
		chunk.RemoveTime(1);

		if (chunk.ShouldUnload())
		{
			std::vector<CachedCollectableBlock> cachedBlocks;
			for (int y = 0; y < chunk.SIZE; ++y)
			{
				for (int x = 0; x < chunk.SIZE; ++x)
				{
					CollectableBlock& block = chunk.GetBlock(x, y);
					if (block.count > 0)
					{
						cachedBlocks.push_back(CachedCollectableBlock{ x, y });
						CachedCollectableBlock& cachedBlock = cachedBlocks[cachedBlocks.size() - 1];
						cachedBlock.cellVector.reserve(block.count);
						for (int i = 0; i < block.count; ++i)
							cachedBlock.cellVector.push_back(block.cellArray[i]);
					}
				}
			}
			if (cachedBlocks.size() > 0)
				unloadedChunks.insert({ iter.first, cachedBlocks });
			toDeleteChunks.push_back(iter.first);
		}
	}
	//LOG_INFO("------------- count: {}", loadedChunks.size());

	for (auto val : toDeleteChunks)
	{
		loadedChunks.erase(val);
	}
	toDeleteChunks.clear();
}

std::pair<int, int> CollectableManager::GetXY(uint64_t hashKey)
{
	int x = static_cast<int>(hashKey >> 32);
	int y = static_cast<int>((hashKey << 32) >> 32);
	return { x, y };
}

void CollectableManager::Debug_RenderChunkBorders(Timestep ts)
{
	// Loaded
	for (auto& [xy, _] : loadedChunks)
	{
		auto [x, y] = GetXY(xy);
		RenderQueue::EnQueue(RenderLayer::FOUR, { x * CollectableChunk::SIZE, y * CollectableChunk::SIZE, RenderDepth::DEBUG_TOP }, Sprites::borderBox, Colour::RED, glm::vec2{ CollectableChunk::SIZE });
	}

	// Unloaded
	for (auto& [xy, _] : unloadedChunks)
	{
		auto [x, y] = GetXY(xy);
		RenderQueue::EnQueue(RenderLayer::FOUR, { x * CollectableChunk::SIZE, y * CollectableChunk::SIZE, RenderDepth::DEBUG_TOP }, Sprites::borderBox, Colour::BLUE, glm::vec2{ CollectableChunk::SIZE });
	}
}
