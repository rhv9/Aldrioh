#pragma once
#include "CollisionChunk.h"
#include <Collision/CollisionDispatcher.h>

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
	inline CollisionChunk& GetChunk(int x, int y) { return chunks[y * width + x]; }
	bool FindAndDispatchCollisions(Timestep ts, Entity e, CollisionDispatcher& dispatcher);

	int GetNumChunkWidth() const { return width; }
	int GetNumChunkHeight() const { return height; }

	glm::vec2 GetMaxActualPosition() const;

	int GetCollisionCheckCount() const { return collisionCheckCount; }
	void ResetCollisionCheckCount() { collisionCheckCount = 0; }



private:
	Scene& scene;
	int width = 1, height = 1;
	float cellSize = 1.0f;
	std::vector<CollisionChunk> chunks;

	std::vector<Cell> cellMap;

	int collisionCheckCount = 0;
};