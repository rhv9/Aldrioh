#pragma once
#include "CollectableChunk.h"

class CollectableManager
{
public:
	CollectableManager(Scene& scene) : scene(scene) {}

	void Init(int numOfChunkWidth, int numOfChunkHeight);
	CollectableMapping GetMapping(const glm::vec2& pos);
	inline CollectableChunk& GetChunk(int x, int y) { return chunks[y * width + x]; }
	inline CollectableChunk& GetChunk(const CollectableMapping& mapping) { return chunks[mapping.chunkY * width + mapping.chunkX]; }

	int GetNumChunkWidth() const { return width; }
	int GetNumChunkHeight() const { return height; }

	glm::vec2 GetMaxActualPosition() const;

private:
	Scene& scene;
	int width = 1, height = 1;
	float cellSize = 1.0f;
	std::vector<CollectableChunk> chunks;
};