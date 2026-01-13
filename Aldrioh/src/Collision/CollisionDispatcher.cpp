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

void CollisionDispatcher::AddCallback(EntityType type1, EntityType type2, CollisionCallbackFunction callback)
{
	ASSERT(type1.id <= SIZE && type2.id <= SIZE, "Size is out of bounds of map");

	map[type1.id][type2.id] = { callback, false };
	map[type2.id][type1.id] = { callback, true };
}

void CollisionDispatcher::Dispatch(CollisionEvent& e1, CollisionEvent& e2)
{
	EntityType type1 = e1.e.GetComponent<EntityTypeComponent>().type;
	EntityType type2 = e2.e.GetComponent<EntityTypeComponent>().type;

	if (map[type1.id][type2.id].reverseArguments)
		map[type1.id][type2.id].callback(e2, e1);
	else
		map[type1.id][type2.id].callback(e1, e2);
}
