#pragma once
#include "CollectableChunk.h"

struct BoundingArea;
class Level;

struct XY
{
	union
	{
		std::pair<int, int> pair{ 0,0 };
		uint64_t hashValue;
	};

	XY() = default;
	XY(uint64_t hash) : hashValue(hash) {}
	XY(const std::pair<int, int>& pair) : pair(pair) {}
};

class CollectableManager
{
public:
	CollectableManager() = default;

	CollectableMapping GetMapping(const glm::vec2& pos);
	CollectableChunk& GetChunk(int x, int y);
	CollectableChunk& GetChunk(const CollectableMapping& mapping) { return GetChunk(mapping.chunkX, mapping.chunkY); }
	CollectableBlock& GetBlock(const CollectableMapping& mapping) { return GetChunk(mapping.chunkX, mapping.chunkY).GetBlock(mapping); }
	CollectableBlock& GetBlock(const glm::vec2& pos) { return GetBlock(GetMapping(pos)); }
	void AddCollectable(const glm::vec2& pos, CollectableType collectableType) { GetBlock(pos).AddCollectable(pos, collectableType); }

	void RenderChunks(CollectableMapping& bottomLeft, CollectableMapping& topRight);
	void OnUpdate(Timestep ts, const CollectableMapping& bottomLeftMapping, const CollectableMapping& topRightMapping);
	
	static std::pair<int, int> GetXY(uint64_t hashKey) { return XY{ hashKey }.pair; }
	static uint64_t GetHashKey(const std::pair<int, int>& pair) { return XY{ pair }.hashValue; }

	void Debug_Render(Level& level, Timestep ts, const CollectableMapping& bottomLeftMapping, const CollectableMapping& topRightMapping);
	void Debug_RenderChunkBorders(Timestep ts);

private:
	float cellSize = 1.0f;

	std::unordered_map<uint64_t, CollectableChunk> loadedChunks;
	std::unordered_map<uint64_t, std::vector<CachedCollectableBlock>> unloadedChunks;
	std::vector<uint64_t> toDeleteChunks;
};