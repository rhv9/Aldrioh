#include <pch.h>
#include "CollisionChunk.h"
#include <Scene/Scene.h>

void Cell::AddEntity(entt::entity handle)
{
	// If already in list, then do not add
	for (int i = 0; i < count; ++i)
		if (handle == entities[i])
			return;

	entities[count++] = handle;
}


CollisionChunk::CollisionChunk(Scene& scene) : scene(scene)
{
}

void CollisionChunk::Clear()
{
	for (Cell& cell : grid)
		cell.count = 0;
}

