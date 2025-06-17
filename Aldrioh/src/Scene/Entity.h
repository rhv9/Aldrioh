#pragma once
#include <entt.hpp>

#include "Scene.h"

class Entity
{
public:
	Entity() = default;
	Entity(entt::entity handle, Scene* scene);
	Entity(const Entity& other) = default;
	~Entity();

	Scene* getScene() { return scene; }

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		CORE_ASSERT(!HasComponent<T>(), "AddComponent: Entity already has component!");

		return scene->registry.emplace<T>(entityHandle, std::forward<Args>(args)...);
	}

	template<typename T>
	T& GetComponent()
	{
		CORE_ASSERT(HasComponent<T>(), "GetComponent: Entity does not have component!");

		return scene->registry.get<T>(entityHandle);
	}

	template<typename T>
	bool HasComponent()
	{
		return scene->registry.all_of<T>(entityHandle);
	}

	template<typename T>
	void RemoveComponent()
	{
		CORE_ASSERT(HasComponent<T>(), "RemoveComponent: Entity does not have component!");

		scene->registry.erase<T>(entityHandle);
	}

	operator bool() const { return entityHandle != entt::null; }

private:
	entt::entity entityHandle = entt::null;
	Scene* scene = nullptr;
};