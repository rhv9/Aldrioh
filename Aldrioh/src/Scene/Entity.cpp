#include <pch.h>
#include "Entity.h"

#include <Scene/Components.h>

Entity Entity::Null{};

Entity::Entity(entt::entity handle, Scene* scene)
{
	this->entityHandle = handle;
	this->scene = scene;
}

Entity::~Entity()
{
}

void Entity::QueueDestroy()
{
	if (!HasComponent<DestroyEntityComponent>())
		AddComponent<DestroyEntityComponent>();
}
