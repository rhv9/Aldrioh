#pragma once
#include "CollectableChunk.h"

struct BoundingArea;

class CollectableManager
{
public:
	CollectableManager() = default;

	void Init(int numOfChunkWidth, int numOfChunkHeight);
	CollectableMapping GetMapping(const glm::vec2& pos);

	inline CollectableChunk& GetChunk(const CollectableMapping& mapping) { return GetChunk(mapping.chunkX, mapping.chunkY); }
	inline CollectableChunk& GetChunk(int x, int y);

	int GetNumChunkWidth() const { return width; }
	int GetNumChunkHeight() const { return height; }

	void RenderChunks(CollectableMapping& bottomLeft, CollectableMapping& topRight);

	void OnUpdate(Timestep ts, const CollectableMapping& bottomLeftMapping, const CollectableMapping& topRightMapping);

private:
	int width = 1, height = 1;
	float cellSize = 1.0f;

	std::unordered_map<uint64_t, CollectableChunk> loadedChunks;
	std::unordered_map<uint64_t, std::vector<CachedCollectableBlock>> unloadedChunks;

	std::vector<uint64_t> toDeleteChunks;

};