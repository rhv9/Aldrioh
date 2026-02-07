#include <pch.h>
#include "CollectableManager.h"

#include <Math/Math.h>
#include <Game.h>

#include <Game/Level/Level.h>

void CollectableManager::Init(int numOfChunkWidth, int numOfChunkHeight)
{
}

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
			LOG_INFO("Loaded Chunk from cache: {},{}, size: ", x, y, unloadedChunks[val].size());
			unloadedChunks.erase(val);
			return loadedChunks[val];
		}
		else
		{
			LOG_INFO("New Chunk: {},{}", x, y);
			loadedChunks.insert({ val, CollectableChunk{} });
			return loadedChunks[val];
		}

	}

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

void CollectableManager::OnUpdate(Timestep ts, const CollectableMapping& bottomLeftMapping, const CollectableMapping& topRightMapping)
{
	// Only run every second
	if (Game::Instance().GetTickCount() % Game::Instance().GetTicksTarget() != 0)
		return;

	auto iter = loadedChunks.begin();
	for (auto& iter : loadedChunks)
	{
		CollectableChunk& chunk = iter.second;

		int x = static_cast<int>(iter.first >> 32);
		int y = static_cast<int>((iter.first << 32) >> 32);

		if (x < bottomLeftMapping.chunkX || x > topRightMapping.chunkX || y < bottomLeftMapping.chunkY || y > topRightMapping.chunkY)
		{
			chunk.ResetTimer();
			continue;
		}

		chunk.RemoveTime(1);

		if (chunk.ShouldUnload())
		{
			LOG_CORE_INFO("LSDKJFLKSDJF");
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
			unloadedChunks.insert({ iter.first, cachedBlocks });
			toDeleteChunks.push_back(iter.first);
		}
	}

	for (auto val : toDeleteChunks)
	{
		loadedChunks.erase(val);
	}
	toDeleteChunks.clear();
}
