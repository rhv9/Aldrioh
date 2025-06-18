#pragma once
#include <entt.hpp>
#include <Game/Entity/EntityType.h>

class Entity;

using CollisionCallbackFunction = std::function<void(Entity& e1, Entity& e2)>;

class CollisionDispatcher
{

public:
	CollisionDispatcher();
	
	void AddCallback(EntityType type1, EntityType type2, CollisionCallbackFunction& callback);
	void Dispatch(Entity e1, Entity e2);

private:
	struct CollisionHandle
	{
		CollisionCallbackFunction callback;
		bool reverseArguments = false;

		CollisionHandle() = default;
		CollisionHandle(const CollisionCallbackFunction& callback, const bool reverse) : callback(callback), reverseArguments(reverse) {}
	};

	static constexpr uint32_t SIZE = 30;
	CollisionHandle map[SIZE][SIZE] = {};
};