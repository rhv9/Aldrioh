#include <pch.h>
#include "CollisionDispatcher.h"
#include <Scene/Components.h>
#include <Scene/Entity.h>

CollisionDispatcher::CollisionDispatcher()
{
	CollisionCallbackFunction emptyCallback = [](CollisionEvent& e1, CollisionEvent& e2) {
		};
	for (int y = 0; y < SIZE; ++y)
	{
		for (int x = 0; x < SIZE; ++x)
		{
			map[y][x] = { emptyCallback, false };
			categoryMap[y][x] = { emptyCallback, false };
		}
	}

}

void CollisionDispatcher::AddCallback(EntityID type1, EntityID type2, CollisionCallbackFunction callback)
{
	ASSERT(type1.id <= SIZE && type2.id <= SIZE, "Size is out of bounds of map");

	map[type1.id][type2.id] = { callback, false };
	map[type2.id][type1.id] = { callback, true };
}

void CollisionDispatcher::AddCallbackCategory(EntityCategory category1, EntityCategory category2, CollisionCallbackFunction callback)
{
	ASSERT(category1 <= SIZE && category2 <= SIZE, "Size is out of bounds of map");
	categoryMap[category1][category2] = { callback, false };
	categoryMap[category2][category1] = { callback, true };
}

void CollisionDispatcher::Dispatch(CollisionEvent& e1, CollisionEvent& e2)
{
	EntityID type1 = e1.e.GetComponent<EntityTypeComponent>().type;
	EntityID type2 = e2.e.GetComponent<EntityTypeComponent>().type;

	if (categoryMap[type1.category][type2.category].reverseArguments)
		categoryMap[type1.category][type2.category].callback(e2, e1);
	else
		categoryMap[type1.category][type2.category].callback(e1, e2);

	if (map[type1.id][type2.id].reverseArguments)
		map[type1.id][type2.id].callback(e2, e1);
	else				   
		map[type1.id][type2.id].callback(e1, e2);
}
