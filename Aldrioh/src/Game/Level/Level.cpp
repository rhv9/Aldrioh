#include "pch.h"
#include "Level.h"

#include "Game/SpriteCollection.h"
#include "Graphics/Renderer.h"

#include "Math/Math.h"
#include <Scene/Components.h>
#include <Scene/FreeRoamEntityCameraController.h>
#include <Input/Input.h>
#include <Game.h>
#include <imgui.h>
#include "Game/RenderDepth.h"
#include "Components/Collision.h"

#include "Core/Platform.h"
#include <Scene/Entity.h>

void CreateBoss(std::shared_ptr<Scene>& scene)
{
	// Create boss
	Entity boss = scene->CreateEntity("Boss");
	boss.GetComponent<TransformComponent>().position = { 5.0f, 5.0f, 0.4f };
	boss.AddComponent<VisualComponent>(Sprites::player_head).localTransform = { -0.5f, -0.5f, 0.0f };
	boss.AddComponent<MoveComponent>(1.0f);
	boss.AddComponent<EntityTypeComponent>(EntityType::Enemy);
	boss.AddComponent<AnimatedMovementComponent>(Sprites::animBossUp, Sprites::animBossDown, Sprites::animBossLeft, Sprites::animBossRight, 0.1f);
	boss.AddComponent<CollisionBox>(glm::vec3{ -0.5f, -0.5f, 0.0f }, glm::vec2{ 1.0f, 1.0f });
	boss.AddComponent<DumbAIComponent>();
}

Level::Level()
{
	scene = std::make_shared<Scene>();

	// collisionDispatcher
	CollisionCallbackType callback = [](entt::registry& registry, entt::entity e1, entt::entity e2) {
		LOG_CORE_INFO("Player colliding with boss!");
		};

	scene->GetCollisionDispatcher().AddCallback(EntityType::Player, EntityType::Enemy, callback);


	// Create player
	Entity player = scene->CreateEntity("Player");
	scene->SetPlayer(player);
	player.GetComponent<TransformComponent>().position = { 0.0f, 0.0f, 0.4f };
	player.AddComponent<VisualComponent>(Sprites::player_head).localTransform = { -0.5f, -0.5f, 0.0f };
	player.AddComponent<MoveComponent>(6.0f);
	player.AddComponent<EntityTypeComponent>(EntityType::Player);
	player.AddComponent<AnimatedMovementComponent>(Sprites::animPlayerUp, Sprites::animPlayerDown, Sprites::animPlayerLeft, Sprites::animPlayerRight, 0.1f);
	player.AddComponent<CollisionBox>(glm::vec3{ -0.5f, -0.5f, 0.0f }, glm::vec2{ 1.0f, 1.0f });
	player.AddComponent<JumpComponent>();

	CreateBoss(scene);


	// Camera
	float aspectRatio = static_cast<float>(Game::Instance().GetWindow()->GetHeight()) / Game::Instance().GetWindow()->GetWidth();
	CameraController* cameraController = new FreeRoamEntityCameraController(aspectRatio, 1.0f);
	cameraController->SetZoomLevel(10);
	cameraController->SetPosition({ cameraController->GetZoomLevel() * 0.75, cameraController->GetZoomLevel() });

	// set free roam entity
	static_cast<FreeRoamEntityCameraController*>(cameraController)->SetEntity(player);

	// Add camera component
	Entity cameraEntity = scene->CreateEntity("RoamAndEntityCamera");
	cameraEntity.AddComponent<CameraComponent>(cameraController);
	cameraEntity.RemoveComponent<TransformComponent>(); // TODO: Need to consider this pls
}


Level::~Level()
{
}

void Level::OnTick(Timestep delta)
{
	scene->OnUpdate(delta);
}


void Level::OnRender(Timestep delta)
{
	scene->OnRender(delta);
}

void Level::OnImGuiRender(Timestep delta)
{
	auto& cameraController = scene->GetPrimaryCameraEntity().GetComponent<CameraComponent>().cameraController;

	ImGui::SetNextWindowBgAlpha(0.6f);
	ImGui::Begin("Level");

	ImGui::SeparatorText("Camera");
	if (ImGui::DragFloat2("pos##1", (float*)&cameraController->GetPosition()))
		cameraController->SetPosition(cameraController->GetPosition());
	if (ImGui::DragFloat("zoom", (float*)&cameraController->GetZoomLevel()))
		cameraController->SetZoomLevel(cameraController->GetZoomLevel());

	ImGui::Text("Bounds: (%.1f, %.1f)", cameraController->GetBounds().Right*2, cameraController->GetBounds().Top*2);

	glm::vec2 mousePos = scene->GetMousePosInScene();
	ImGui::Text("Mouse in world: (%.2f, %.2f)", mousePos.x, mousePos.y);

	ImGui::SeparatorText("Player");
	ImGui::DragFloat3("pos##2", (float*)&scene->GetPlayer()->GetComponent<TransformComponent>().position);
	ImGui::DragFloat("z", &scene->GetPlayer()->GetComponent<JumpComponent>().z);
	ImGui::DragFloat("velocity", &scene->GetPlayer()->GetComponent<JumpComponent>().velocity);
	ImGui::DragFloat("acceleration", &scene->GetPlayer()->GetComponent<JumpComponent>().acceleration);
	ImGui::Text("moving: %s", scene->GetPlayer()->GetComponent<MoveComponent>().isMoving() ?"Yes" : "No");

	ImGui::SeparatorText("Entity");
	ImGui::Text("Entity count: %d", scene->Registry().view<TransformComponent>().size());

	ImGui::End();
	ImGui::SetNextWindowBgAlpha(1.0f);
}
