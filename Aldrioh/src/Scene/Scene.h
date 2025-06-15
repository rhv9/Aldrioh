#pragma once
#include <entt.hpp>

class Scene
{
public:
	Scene();
	~Scene();

	// temp
	entt::registry& Registry() { return m_Registry; }

	Entity CreateEntity(const std::string& name = std::string("Entity"));
	void OnUpdate(Timestep ts);

private:
	entt::registry m_Registry;

	friend class Entity;
};
