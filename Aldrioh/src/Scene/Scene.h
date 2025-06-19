#pragma once
#include <entt.hpp>
#include <Components/CollisionDispatcher.h>

class Entity;

class Scene
{
	using SystemFunction = std::function<void(Timestep, Scene&)>;
public:
	Scene();
	~Scene();

	Entity CreateEntity(const std::string& name = std::string("Entity"));

	void OnUpdate(Timestep ts);
	void OnRender(Timestep ts);

	void SetPlayer(const Entity& e);
	Entity* GetPlayer() { return player; }

	void AddSystem(const SystemFunction& callback);
	
	// temp
	entt::registry& Registry() { return registry; }
	Entity GetPrimaryCameraEntity();
	glm::vec2 GetMousePosInScene();
	CollisionDispatcher& GetCollisionDispatcher();

	Entity WrapEntityHandle(entt::entity entityHandle);

private:

	entt::registry registry;
	Entity* player = nullptr;

	CollisionDispatcher collisionDispatcher;
	
	std::vector<SystemFunction> systems;

	friend class Entity;
};
