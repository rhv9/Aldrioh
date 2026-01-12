#pragma once
#include "CollisionChunk.h"

class CollisionWorld
{
public:
	CollisionWorld() {}
	
	void Init(int numOfChunkWidth, int numOfChunkHeight, int chunkWidth, int chunkHeight);

private:
	int width = 1, height = 1;
	int chunkWidth = 1, chunkHeight = 1;
	std::vector<CollisionChunk> chunks;
};