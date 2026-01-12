#pragma once
#include "CollisionChunk.h"

struct PositionMapping
{
	int chunkX = 0, chunkY = 0;
	int cellX = 0, cellY = 0;

	std::string ToString() const;
};

class CollisionWorld
{
public:
	CollisionWorld(Scene& scene) : scene(scene) {}
	
	void Init(int numOfChunkWidth, int numOfChunkHeight);
	PositionMapping GetMapping(const glm::vec2& pos);

private:
	Scene& scene;
	int width = 1, height = 1;
	std::vector<CollisionChunk> chunks;
};