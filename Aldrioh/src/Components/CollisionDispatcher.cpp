#include <pch.h>
#include "CollisionDispatcher.h"
#include <Scene/Components.h>
#include <Scene/Entity.h>

CollisionDispatcher::CollisionDispatcher()
{
	CollisionCallbackFunction emptyCallback = [](CollisionEvent& e1, CollisionEvent& e2) {
		};
	for (int y = 0; y < SIZE; ++y)
		for (int x = 0; x < SIZE; ++x)
			map[y][x] = { emptyCallback, false };
}

void CollisionDispatcher::AddCallback(EntityType type1, EntityType type2, CollisionCallbackFunction& callback)
{
	ASSERT(type1 <= SIZE && type2 <= SIZE, "Size is out of bounds of map");

	map[type1][type2] = { callback, false };
	map[type2][type1] = { callback, true };
}

void CollisionDispatcher::Dispatch(CollisionEvent& e1, CollisionEvent& e2)
{
	EntityType type1 = e1.e.GetComponent<EntityTypeComponent>().type;
	EntityType type2 = e2.e.GetComponent<EntityTypeComponent>().type;

	if (map[type1][type2].reverseArguments)
		map[type1][type2].callback(e2, e1);
	else
		map[type1][type2].callback(e1, e2);
}
