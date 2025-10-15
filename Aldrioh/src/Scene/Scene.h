#pragma once
#include <entt.hpp>
#include <Components/CollisionDispatcher.h>
#include <UI/UIManager.h>

class Entity;

class Scene
{
	using SystemFunction = std::function<void(Timestep, Scene&)>;
public:
	Scene();
	~Scene();

	Entity CreateEntity(const std::string& name = std::string("Entity"));
	Entity CreateEntityNoTransform(const std::string& name = std::string("Entity"));
	void DestroyEntity(Entity& e);

	void OnUpdate(Timestep ts);
	void OnRender(Timestep ts);
	void OnUIRender(Timestep ts);

	void SetPlayer(const Entity& e);
	Entity* GetPlayer() { return player; }

	void AddUpdateSystem(const SystemFunction& callback) { updateSystems.push_back(callback); }
	void AddRenderSystem(const SystemFunction& callback) { renderSystems.push_back(callback); }
	void AddUIRenderSystem(const SystemFunction& callback) { uiRenderSystems.push_back(callback); }

	// temp
	entt::registry& getRegistry() { return registry; }
	Entity GetPrimaryCameraEntity();
	glm::vec2 GetMousePosInScene();
	CollisionDispatcher& GetCollisionDispatcher();

	Entity WrapEntityHandle(entt::entity entityHandle);

	void OnTransitionIn();
	void OnTransitionOut();

private:
	entt::registry registry;
	Entity* player = nullptr;

	CollisionDispatcher collisionDispatcher;

	std::vector<SystemFunction> updateSystems;
	std::vector<SystemFunction> renderSystems;
	std::vector<SystemFunction> uiRenderSystems;

	friend class Entity;
};
