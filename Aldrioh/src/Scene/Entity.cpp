#include <pch.h>
#include "Entity.h"

Entity::Entity(entt::entity handle, Scene* scene)
{
	this->entityHandle = handle;
	this->scene = scene;
}

Entity::~Entity()
{
}
