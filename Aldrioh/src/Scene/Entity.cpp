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

TransformComponent& Entity::GetTransformComponent()
{
	CORE_ASSERT(HasComponent<TransformComponent>(), "GetComponent: Entity does not have component!");
	return scene->registry.get<TransformComponent>(entityHandle);
}

void Entity::QueueDestroy()
{
	if (!HasComponent<DestroyEntityComponent>())
		AddComponent<DestroyEntityComponent>();
}
