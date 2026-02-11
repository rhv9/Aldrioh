#include <pch.h>
#include "CollectableManager.h"

#include <Math/Math.h>
#include <Game.h>
#include <Game/Level/Level.h>

#include <Graphics/RenderQueue.h>
#include <Game/SpriteCollection.h>
#include <UI/Colour.h>
#include <Game/RenderDepth.h>

#include <Game/Debug/GameDebugState.h>
#include <Scene/Components.h>
#include <Game/Components/ControllerComponents.h>
#include <Game/Systems/RenderSystems.h>

CollectableMapping CollectableManager::GetMapping(const glm::vec2& pos)
{
	float chunkSize = CollectableChunk::SIZE;
	glm::vec2 dividedPos = pos / chunkSize;
	CollectableMapping mapping;
	mapping.chunkX = Math::ffloor(dividedPos.x);
	mapping.chunkY = Math::ffloor(dividedPos.y);
	mapping.cellX = static_cast<int>(Math::floatModNegative(pos.x, chunkSize));
	mapping.cellY = static_cast<int>(Math::floatModNegative(pos.y, chunkSize));

	return mapping;
}


inline CollectableChunk& CollectableManager::GetChunk(int x, int y)
{
	uint64_t val = GetHashKey({ x, y });

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

void CollectableManager::Debug_Render(Level& level, Timestep ts, const CollectableMapping& bottomLeftMapping, const CollectableMapping& topRightMapping)
{
	glm::vec2 playerCameraPos = level.GetPlayerCamera().GetComponent<CameraComponent>().cameraController->GetPosition();
	glm::vec2 playerPos = EntitySystem::CalculateEntityTransformWithInterpolation(level.GetPlayer(), ts);

	auto& levelArea = level.GetScreenBorderOffset();

	if (GameDebugState::renderChunkBordersBeingRendered)
	{
		for (int y = bottomLeftMapping.chunkY; y < topRightMapping.chunkY + 1; ++y)
			for (int x = bottomLeftMapping.chunkX; x < topRightMapping.chunkX + 1; ++x)
				RenderQueue::EnQueue(RenderLayer::FOUR, { x * CollectableChunk::SIZE, y * CollectableChunk::SIZE, 0.8f }, Sprites::borderBox, Colour::WHITE, { CollectableChunk::SIZE, CollectableChunk::SIZE });
	}

	if (GameDebugState::showLoadedAndUnloadedCollectableChunks)
	{
		Debug_RenderChunkBorders(ts);
	}

	if (GameDebugState::renderCollectableCells)
	{
		// Rendering jewels in the screen
		int startX = static_cast<int>(levelArea.bottomLeft.x + playerCameraPos.x);
		int startY = static_cast<int>(levelArea.bottomLeft.y + playerCameraPos.y);
		int endX = static_cast<int>(levelArea.topRight.x + playerCameraPos.x + 1);
		int endY = static_cast<int>(levelArea.topRight.y + playerCameraPos.y + 1);

		for (int y = startY; y < endY; ++y)
			for (int x = startX; x < endX; ++x)
				RenderQueue::EnQueue(RenderLayer::FOUR, { x, y, RenderDepth::COLLECTABLES }, Sprites::borderBox, Colour::GREEN);

		// Render cells that player is collecting from
		{
			auto& pcc = level.GetPlayer().GetComponent<PlayerControllerComponent>();
			int startX = static_cast<int>(playerPos.x - pcc.radius);
			int startY = static_cast<int>(playerPos.y - pcc.radius);
			int endX = static_cast<int>(playerPos.x + pcc.radius);
			int endY = static_cast<int>(playerPos.y + pcc.radius);

			for (int y = startY; y < endY; ++y)
				for (int x = startX; x < endX; ++x)
					if (Math::dist(glm::vec2{ (float)x + 0.5f, (float)y + 0.5f }, playerPos) <= pcc.radius)
						RenderQueue::EnQueue(RenderLayer::FOUR, glm::vec3{ x, y, RenderDepth::COLLECTABLES }, Sprites::borderBox, Colour::BLUE);
		}
	}
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
