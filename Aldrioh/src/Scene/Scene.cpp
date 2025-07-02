#include "pch.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"
#include "Math/Math.h"

#include <Input/Input.h>
#include <Game.h>

#include <Components/Collision.h>
#include <Components/CollisionDispatcher.h>

#include <Graphics/Renderer.h>
#include <Graphics/RenderQueue.h>
#include <Game/SpriteCollection.h>
#include <Core/Platform.h>
#include "Game/RenderDepth.h"

#include <Game/Systems/PlayerControllerSystems.h>

Scene::Scene()
{
}

Scene::~Scene()
{
	delete player;
}

Entity Scene::CreateEntity(const std::string& name)
{
	Entity e = { registry.create(), this };
	e.AddComponent<NameComponent>(name);
	e.AddComponent<TransformComponent>();
	return e;
}

void Scene::DestroyEntity(Entity& e)
{
	registry.destroy(e.entityHandle);
	e.entityHandle = entt::null;
}

void Scene::SetPlayer(const Entity& e)
{
	if (!player) delete player;
	player = new Entity(e);
}

void Scene::AddUpdateSystem(const SystemFunction& callback)
{
	updateSystems.push_back(callback);
}

void Scene::AddRenderSystem(const SystemFunction& callback)
{
	renderSystems.push_back(callback);
}

void Scene::OnUpdate(Timestep ts)
{
	// Run each system
	for (auto system : updateSystems)
		system(ts, *this);

	auto cameraEntity = GetPrimaryCameraEntity();
	cameraEntity.GetComponent<CameraComponent>().cameraController->OnUpdate(ts);

}

void Scene::OnRender(Timestep ts)
{
	auto& cameraController = GetPrimaryCameraEntity().GetComponent<CameraComponent>().cameraController;
	Renderer::StartScene(cameraController->GetCamera().GetViewProjection());
	RenderQueue::Reset();

	for (auto system : renderSystems)
		system(ts, *this);
	
	RenderQueue::FlushAndReset();
	Renderer::EndScene();
}

Entity Scene::WrapEntityHandle(entt::entity entityHandle)
{
	return { entityHandle, this };
}

Entity Scene::GetPrimaryCameraEntity()
{
	auto view = registry.view<CameraComponent>();

	// TODO: Currently accepts only one camera as main camera, update to support more
	for (entt::entity e : view)
	{
		return Entity(e, this);
	}

	LOG_CORE_CRITICAL("No camera added to scene!");

	return Entity();
}

glm::vec2 Scene::GetMousePosInScene()
{
	glm::vec2 mousePos = Input::GetMousePosition();
	auto& cameraComponent = GetPrimaryCameraEntity().GetComponent<CameraComponent>();
	glm::vec2 cameraPos = cameraComponent.cameraController->GetPosition();
	auto& bounds = cameraComponent.cameraController->GetBounds();

	glm::vec2 mousePosPercent = { mousePos.x / Game::Instance().GetWindow()->GetWidth(),
						 mousePos.y / Game::Instance().GetWindow()->GetHeight() };


	glm::vec2 zoomedDimensions{ bounds.Right, -bounds.Top };

	return (glm::vec2{ mousePosPercent.x * zoomedDimensions.x - bounds.Right / 2.0f, mousePosPercent.y * zoomedDimensions.y + bounds.Top / 2.0f } *2.0f) + cameraComponent.cameraController->GetPosition();
}

CollisionDispatcher& Scene::GetCollisionDispatcher()
{
	return collisionDispatcher;
}

