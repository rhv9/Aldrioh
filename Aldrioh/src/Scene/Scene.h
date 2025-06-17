#pragma once
#include <entt.hpp>

class Entity;

class Scene
{
public:
	Scene();
	~Scene();

	// temp
	entt::registry& Registry() { return registry; }

	Entity CreateEntity(const std::string& name = std::string("Entity"));
	void OnUpdate(Timestep ts);

	Entity wrapEntityHandle(entt::entity entityHandle);

private:
	entt::registry registry;

	friend class Entity;
};
