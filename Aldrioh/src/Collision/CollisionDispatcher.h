#pragma once
#include <entt.hpp>
#include <Game/Entity/EntityType.h>

class Entity;

struct CollisionEvent
{
	Entity& e;
	bool handled = false;
};

using CollisionCallbackFunction = std::function<void(CollisionEvent& e1, CollisionEvent& e2)>;

class CollisionDispatcher
{

public:
	CollisionDispatcher();
	
	void AddCallback(EntityID type1, EntityID type2, CollisionCallbackFunction callback);
	void AddCallbackCategory(EntityCategory category1, EntityCategory category2, CollisionCallbackFunction callback);
	void Dispatch(CollisionEvent& e1, CollisionEvent& e2);

private:
	struct CollisionHandle
	{
		CollisionCallbackFunction callback;
		bool reverseArguments = false;

		CollisionHandle() = default;
		CollisionHandle(const CollisionCallbackFunction& callback, const bool reverse) : callback(callback), reverseArguments(reverse) {}
	};

	static constexpr uint32_t SIZE = 48;
	CollisionHandle map[SIZE][SIZE] = {};
	CollisionHandle categoryMap[SIZE][SIZE] = {};

};