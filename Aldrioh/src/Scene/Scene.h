#pragma once
#include <entt.hpp>
#include <Collision/CollisionDispatcher.h>
#include <Collision/CollisionZone.h>
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
	Entity& GetPlayer() { return *player; }

	void AddUpdateSystem(const SystemFunction& callback) { updateSystems.push_back(callback); }
	void AddRenderSystem(const SystemFunction& callback) { renderSystems.push_back(callback); }
	void AddUIRenderSystem(const SystemFunction& callback) { uiRenderSystems.push_back(callback); }

	// temp
	entt::registry& getRegistry() { return registry; }
	glm::vec2 GetMousePosInScene();

	CollisionDispatcher& GetCollisionDispatcher();

	bool DispatchCollisions(Timestep ts, Entity e);
	CollisionZone& GetCollisionZone() { return collisionZone; }

	Entity GetPrimaryCameraEntity();
	void SetPrimaryCameraEntity(Entity primaryEntity);

	Entity WrapEntityHandle(entt::entity entityHandle);

	ParticleManager& GetParticleManager() { return particleManager; }

	void OnTransitionIn();
	void OnTransitionOut();

	void OnMouseButtonEvent(MouseButtonEventArg& e);
	void OnMouseMoveEvent(MouseMoveEventArg& e);
	void OnMouseScrollEvent(MouseScrolledEventArg& e);
	void OnWindowResizeEvent(WindowResizeEventArg& e);
	void OnKeyEvent(KeyEventArg& e);

	template<typename T>
	Entity GetFirstEntity()
	{
		auto view = registry.view<T>();

		for (entt::entity e : view)
			return Entity{ e, this };

		return Entity{};
	}

	template<typename T>
	T& GetFirstComponent()
	{
		auto view = registry.view<T>();

		for (entt::entity e : view)
			return view.get<T>(e);

		ASSERT(false, "No entity found with component T");
		return registry.get<T>(entt::null); // I don't like this tbh but I am hoping this would never need to run. Otherwise could use ptr or std::optional
	}

private:
	int entityCount = 0;
	entt::registry registry;
	Entity* player = nullptr;
	
	CollisionDispatcher collisionDispatcher;
	CollisionZone collisionZone;

	ParticleManager particleManager;

	std::vector<SystemFunction> updateSystems;
	std::vector<SystemFunction> renderSystems;
	std::vector<SystemFunction> uiRenderSystems;

	friend class Entity;
};
