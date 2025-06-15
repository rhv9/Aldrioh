#include <pch.h>
#include "CollisionDispatcher.h"
#include <Scene/Components.h>

CollisionDispatcher::CollisionDispatcher()
{
	for (int y = 0; y < SIZE; ++y)
		for (int x = 0; x < SIZE; ++x)
			map[y][x] = nullptr;
}

// TODO: This is dangerous. No map validation or anything
void CollisionDispatcher::AddCallback(EntityType type1, EntityType type2, CollisionCallbackType callback)
{
	map[(uint32_t)type1][(uint32_t)type2] = callback;
	map[(uint32_t)type2][(uint32_t)type1] = callback;
}

bool CollisionDispatcher::Dispatch(entt::registry& registry, entt::entity e1, entt::entity e2)
{
	EntityType type1 = registry.get<EntityTypeComponent>(e1).type;
	EntityType type2 = registry.get<EntityTypeComponent>(e2).type;

	map[type1][type2](registry, e1, e2);
	return false;
}
