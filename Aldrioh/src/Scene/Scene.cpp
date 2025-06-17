#include "pch.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

Entity Scene::CreateEntity(const std::string& name)
{
	Entity e = { registry.create(), this };
	e.AddComponent<NameComponent>(name);
	e.AddComponent<TransformComponent>();
	return e;
}

Entity Scene::wrapEntityHandle(entt::entity entityHandle)
{
	return { entityHandle, this };
}
