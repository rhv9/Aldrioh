#include <pch.h>
#include "CollisionWorld.h"

void CollisionWorld::Init(int numOfChunkWidth, int numOfChunkHeight, int chunkWidth, int chunkHeight)
{
	width = numOfChunkWidth;
	height = numOfChunkHeight;
	this->chunkWidth = chunkWidth;
	this->chunkHeight = chunkHeight;

	chunks.reserve(width * height);

	for (auto& chunk : chunks)
		chunk.Init(chunkWidth, chunkHeight);
}
