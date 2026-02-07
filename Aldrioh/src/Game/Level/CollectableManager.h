#pragma once
#include "CollectableChunk.h"

struct BoundingArea;


struct XY
{
	union
	{
		std::pair<int, int> pair{ 0,0 };
		uint64_t hashValue;
	};
};

class CollectableManager
{
public:
	CollectableManager() = default;

	CollectableMapping GetMapping(const glm::vec2& pos);
	inline CollectableChunk& GetChunk(const CollectableMapping& mapping) { return GetChunk(mapping.chunkX, mapping.chunkY); }
	inline CollectableChunk& GetChunk(int x, int y);

	void RenderChunks(CollectableMapping& bottomLeft, CollectableMapping& topRight);
	void OnUpdate(Timestep ts, const CollectableMapping& bottomLeftMapping, const CollectableMapping& topRightMapping);
	static std::pair<int, int> GetXY(uint64_t hashKey);

	void Debug_RenderChunkBorders(Timestep ts);

private:
	float cellSize = 1.0f;

	std::unordered_map<uint64_t, CollectableChunk> loadedChunks;
	std::unordered_map<uint64_t, std::vector<CachedCollectableBlock>> unloadedChunks;
	std::vector<uint64_t> toDeleteChunks;
};