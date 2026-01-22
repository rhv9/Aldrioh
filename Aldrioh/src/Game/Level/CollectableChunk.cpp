#include <pch.h>
#include "CollectableChunk.h"
#include <Scene/Scene.h>


CollisionChunk::CollisionChunk(Scene& scene) : scene(scene)
{
}

void CollisionChunk::Clear()
{
	for (Cell& cell : grid)
		cell.count = 0;
}

void CollectableCell::AddCollectable(uint8_t x, uint8_t y, CCellType type)
{
	cellArray[count++] = { x, y, type };
}
