#include "pch.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"
#include "Math/Math.h"

#include <Input/Input.h>
#include <Game.h>

#include <Collision/Collision.h>
#include <Collision/CollisionDispatcher.h>

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

Entity Scene::CreateEntityNoTransform(const std::string& name)
{
	Entity e = { registry.create(), this };
	e.AddComponent<NameComponent>(name);
	return e;
}

void Scene::DestroyEntity(Entity& e)
{
	registry.destroy(e.entityHandle);
	e.entityHandle = entt::null;
}

void Scene::SetPlayer(const Entity& e)
{
	delete player;
	player = new Entity(e);
}

void Scene::OnUpdate(Timestep ts)
{
	// Run each system
	for (auto system : updateSystems)
		system(ts, *this);

}

void Scene::OnRender(Timestep ts)
{
	auto& cameraController = GetPrimaryCameraEntity().GetComponent<CameraComponent>().cameraController;
	cameraController->OnUpdate(ts);

	Renderer::StartScene({ cameraController->GetCamera().GetViewProjection() });
	Renderer::ClearDepthBuffer();
	RenderQueue::Reset();

	for (auto system : renderSystems)
		system(ts, *this);

	SubTexture subTexture = Renderer::GetBackgroundPassTexture()->GetAsSubTexture();
	Renderer::DrawQuad(glm::vec3{ 3, 3, 0 }, &subTexture, { 5, 5 });

	RenderQueue::Flush();
	Renderer::EndScene();
}

void Scene::OnUIRender(Timestep ts)
{
	Renderer::StartUIScene();

	for (auto system : uiRenderSystems)
		system(ts, *this);

	Renderer::EndUIScene();
}

Entity Scene::WrapEntityHandle(entt::entity entityHandle)
{
	return { entityHandle, this };
}

// Temporarily hard coded for simplicity
void Scene::OnTransitionIn()
{
	auto view = getRegistry().view<UIManagerComponent>();

	for (entt::entity e : view)
	{
		UIManagerComponent& uimc = view.get<UIManagerComponent>(e);
		uimc.uiManager->AttachEventListeners();
	}
}

// Temporarily hard coded for simplicity
void Scene::OnTransitionOut()
{
	auto view = getRegistry().view<UIManagerComponent>();

	for (entt::entity e : view)
	{
		UIManagerComponent& uimc = view.get<UIManagerComponent>(e);
		uimc.uiManager->DetachEventListeners();
	}
}

Entity Scene::GetPrimaryCameraEntity()
{
	auto view = registry.view<CameraComponent>();

	// TODO: Currently accepts only one camera as main camera, update to support more
	for (entt::entity e : view)
	{
		CameraComponent& cc = view.get<CameraComponent>(e);
		if (cc.primary)
			return Entity(e, this);
	}

	// If primary not set, then auto set the first camera
	for (entt::entity e : view)
	{
		CameraComponent& cc = view.get<CameraComponent>(e);
		cc.primary = true;
		return Entity(e, this);
	}

	LOG_CORE_CRITICAL("No camera added to scene!");

	return Entity();
}

void Scene::SetPrimaryCameraEntity(Entity primaryEntity)
{
	bool found = false;
	auto view = registry.view<CameraComponent>();

	for (auto e : view)
	{
		CameraComponent& cc = view.get<CameraComponent>(e);
		if (e == primaryEntity.entityHandle)
		{
			cc.primary = true;
			found = true;
		}
		else
			cc.primary = false;
	}

	if (!found)
		LOG_CORE_ERROR("Primary Camera not found when trying to set!");

}

glm::vec2 Scene::GetMousePosInScene()
{
	glm::vec2 mousePos = Input::GetMousePosition();
	auto& cameraComponent = GetPrimaryCameraEntity().GetComponent<CameraComponent>();
	glm::vec2 cameraPos = cameraComponent.cameraController->GetPosition();
	auto& bounds = cameraComponent.cameraController->GetBounds();

	glm::vec2 mousePosPercent = { mousePos.x / Game::Instance().GetWindow()->GetWidth(),
						 mousePos.y / Game::Instance().GetWindow()->GetHeight() };

	glm::vec2 zoomedDimensions{ bounds.Right, bounds.Top };

	return (glm::vec2{ mousePosPercent.x * zoomedDimensions.x - bounds.Right / 2.0f, mousePosPercent.y * zoomedDimensions.y - bounds.Top / 2.0f } *2.0f) + cameraComponent.cameraController->GetPosition();
}

CollisionDispatcher& Scene::GetCollisionDispatcher()
{
	return collisionDispatcher;
}

void Scene::InitCollisionWorldSize(int numOfChunkWidth, int numOfChunkHeight, int chunkWidth, int chunkHeight)
{
	collisionWorld.Init(numOfChunkWidth, numOfChunkHeight, chunkWidth, chunkHeight);
}

bool Scene::HandleCollisions(Entity e, CollisionDispatcher& dispatcher)
{
	

	return false;
}

