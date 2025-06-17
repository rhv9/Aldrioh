#pragma once
#include <entt.hpp>
#include <Game/Entity/EntityType.h>

using CollisionCallbackType = std::function<void(entt::registry&, entt::entity, entt::entity)>;

class CollisionDispatcher
{
public:
	CollisionDispatcher();
	
	void AddCallback(EntityType type1, EntityType type2, CollisionCallbackType& callback);
	bool Dispatch(entt::registry& registry, entt::entity e1, entt::entity e2);

private:
	static constexpr uint32_t SIZE = 30;
	CollisionCallbackType map[SIZE][SIZE] = {};
};