#include <pch.h>
#include "CollisionChunk.h"
#include <Scene/Scene.h>

CollisionChunk::CollisionChunk(Scene& scene) : scene(scene)
{
}

Cell& CollisionChunk::GetCell(const glm::vec2& entityPos)
{
	int x = (int)entityPos.x;
	int y = (int)entityPos.y;
	return grid[y * width + x];
}

void CollisionChunk::SetSize(int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;

	grid.reserve(width * height);
}
