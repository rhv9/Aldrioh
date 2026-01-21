#pragma once
#include <entt.hpp>
#include <Collision/CollisionDispatcher.h>
#include <Collision/CollisionWorld.h>
#include <UI/UIManager.h>

#include "ParticleManager.h"

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

	void OnImGuiDebugRender(Timestep ts);

	void SetPlayer(const Entity& e);
	Entity* GetPlayer() { return player; }

	void AddUpdateSystem(const SystemFunction& callback) { updateSystems.push_back(callback); }
	void AddRenderSystem(const SystemFunction& callback) { renderSystems.push_back(callback); }
	void AddUIRenderSystem(const SystemFunction& callback) { uiRenderSystems.push_back(callback); }

	// temp
	entt::registry& getRegistry() { return registry; }
	glm::vec2 GetMousePosInScene();

	CollisionDispatcher& GetCollisionDispatcher();
	void InitCollisionWorldSize(int numOfChunkWidth, int numOfChunkHeight);
	bool DispatchCollisions(Entity e);
	CollisionWorld& GetCollisionWorld() { return collisionWorld; }

	Entity GetPrimaryCameraEntity();
	void SetPrimaryCameraEntity(Entity primaryEntity);

	Entity WrapEntityHandle(entt::entity entityHandle);

	ParticleManager& GetParticleManager() { return particleManager; }

	void OnTransitionIn();
	void OnTransitionOut();

	template<typename T>
	Entity GetFirstEntity()
	{
		auto view = registry.view<T>();

		for (entt::entity e : view)
			return Entity{ e, this };

		return Entity{};
	}

private:
	int entityCount = 0;
	entt::registry registry;
	Entity* player = nullptr;
	
	CollisionDispatcher collisionDispatcher;
	CollisionWorld collisionWorld;

	ParticleManager particleManager;

	std::vector<SystemFunction> updateSystems;
	std::vector<SystemFunction> renderSystems;
	std::vector<SystemFunction> uiRenderSystems;

	friend class Entity;
};
