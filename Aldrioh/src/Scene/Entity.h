#pragma once
#include <entt.hpp>

#include "Scene.h"

class Entity
{
public:
	Entity() = default;
	Entity(entt::entity handle, Scene* scene);
	Entity(const Entity& other) = default;

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		LOG_CORE_CRITICAL_IF(!HasComponent<T>(), "Entity already has component!");

		return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
	}

	template<typename T>
	T& GetComponent()
	{
		LOG_CORE_CRITICAL_IF(HasComponent<T>(), "Entity does not have component!");

		return m_Scene->m_Registry.get<T>(m_EntityHandle);
	}

	template<typename T>
	bool HasComponent()
	{
		return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
	}

	template<typename T>
	void RemoveComponent()
	{
		LOG_CORE_CRITICAL_IF(HasComponent<T>(), "Entity does not have component!");

		m_Scene->m_Registry.erase<T>(m_EntityHandle);
	}

	operator bool() const { return entityHandle != entt::null; }

private:
	entt::entity entityHandle = entt::null;
	Scene* scene = nullptr;
};