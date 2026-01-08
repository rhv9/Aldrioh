#include <pch.h>
#include "CollisionGrid.h"
#include <Scene/Scene.h>

CollisionGrid::CollisionGrid(Scene& scene) : scene(scene)
{
}

Cell& CollisionGrid::GetCell(const glm::vec2& entityPos)
{
	int x = (int)entityPos.x;
	int y = (int)entityPos.y;
	return grid[y * width + x];
}

void CollisionGrid::SetSize(int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;

	grid.reserve(width * height);
}
