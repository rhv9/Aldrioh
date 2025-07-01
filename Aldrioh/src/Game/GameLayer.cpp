#include "pch.h"
#include "GameLayer.h"

#include "Game.h"
#include "Core/Platform.h"
#include "Input/Input.h"
#include "Graphics/Renderer.h"

#include "Game/SpriteCollection.h"
#include "RenderDepth.h"

#include <imgui.h>

#include "Math/Math.h"
#include <Scene/Components.h>
#include <Scene/FreeRoamEntityCameraController.h>
#include "Components/Collision.h"

#include <Scene/Entity.h>

#include <Game/Systems/CollisionSystems.h>
#include <Game/Systems/AnimatedSystems.h>
#include <Game/Systems/MovementSystems.h>
#include <Game/Systems/EnemyAISystems.h>
#include <Game/Systems/PlayerControllerSystems.h>
#include <Game/Systems/TestSystems.h>
#include <Game/Systems/CoreEntitySystems.h>

#include <Game/Systems/LevelSystems.h>
#include <Game/Systems/RenderSystems.h>

#include <Game/Level/TestLevel.h>
#include <Game/Components/LevelComponents.h>

GameLayer::GameLayer() {}



void GameLayer::OnBegin()
{
	Sprites::init();

	scene = std::make_shared<Scene>();

	// collisionDispatcher
	CollisionCallbackFunction callback = [](Entity& e1, Entity& e2) {
		LOG_CORE_INFO("{} collides {}", e1.GetComponent<NameComponent>().name, e2.GetComponent<NameComponent>().name);
		};

	scene->GetCollisionDispatcher().AddCallback(EntityType::Player, EntityType::Enemy, callback);


	CollisionCallbackFunction callbackFireball = [](Entity& e1, Entity& e2) {
		e1.Destroy();
		e2.Destroy();
		};

	scene->GetCollisionDispatcher().AddCallback(EntityType::Fireball, EntityType::Enemy, callbackFireball);


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

	// Level system
	Entity levelEntity = scene->CreateEntity("Test Level");
	levelEntity.AddComponent<LevelComponent>(new TestLevel(*scene));
	levelEntity.RemoveComponent<TransformComponent>(); // TODO: Need to consider this pls

	// On Update Systems
	scene->AddUpdateSystem(&EntitySystem::ResetMovementSystem);
	scene->AddUpdateSystem(&EntitySystem::PlayerControllerSystem);
	scene->AddUpdateSystem(&EntitySystem::JumpSystem);
	scene->AddUpdateSystem(&EntitySystem::LifeSystem);
	scene->AddUpdateSystem(&EntitySystem::DumbAISystem);
	scene->AddUpdateSystem(&EntitySystem::AnimatedMovementSystem);
	scene->AddUpdateSystem(&EntitySystem::LevelUpdateSystem);
	scene->AddUpdateSystem(&EntitySystem::TestUpdateSystem);
	scene->AddUpdateSystem(&EntitySystem::MovementSystem);
	scene->AddUpdateSystem(&EntitySystem::CollisionSystem);
	scene->AddUpdateSystem(&EntitySystem::CoreEntitySystems);


	// On Render Systems
	scene->AddRenderSystem(&EntitySystem::LevelRenderSystem);
	scene->AddRenderSystem(&EntitySystem::TestRenderSystem);
	scene->AddRenderSystem(&EntitySystem::EntityRenderSystem);
	scene->AddRenderSystem(&EntitySystem::CollisionRenderSystem);
}

void GameLayer::OnUpdate(Timestep delta)
{
	scene->OnUpdate(delta);
	scene->OnRender(delta);
}

void GameLayer::OnImGuiRender(Timestep delta)
{
	Game& game = Game::Instance();
	ImGuiIO& io = ImGui::GetIO();

	ImGui::SetNextWindowBgAlpha(0.6f);
	ImGui::Begin("Main Window");

	ImGui::SeparatorText("Game/Window");
	ImGui::Text("frame time: %.2f (%dfps)", delta.GetMilliSeconds(), game.gameStats.fps);
	ImGui::Text("Elapsed time: %.2f", Platform::GetElapsedTime());
	ImGui::Text("Blocking events: %s", ImGui::IsWindowFocused() ? "Yes" : "No");
	bool vsync = game.GetWindow()->GetVsync();
	if (ImGui::Checkbox("vsync", &vsync))
		game.GetWindow()->SetVsync(vsync);
	bool navActive = io.NavActive;
	ImGui::Checkbox("ImGui Nav Active", &navActive);
	bool wantCaptureKeyboard = io.WantCaptureKeyboard;
	ImGui::Checkbox("ImGui capture keyboard", &wantCaptureKeyboard);
	bool wantCaptureMouse = io.WantCaptureMouse;
	ImGui::Checkbox("ImGui capture mouse", &wantCaptureMouse);


	ImGui::SeparatorText("Renderer");
	bool renderDepth = Renderer::IsRenderDepth();
	if (ImGui::Checkbox("Render depth", &renderDepth))
		Renderer::SetRenderDepthOnly(renderDepth);
	ImGui::SliderFloat("Tile", &RenderDepth::TILE, 0.0f, 1.0f);
	ImGui::SliderFloat("Entity", &RenderDepth::ENTITY, 0.0f, 1.0f);

	ImGui::SeparatorText("Input");
	ImGui::Text("Mouse Pos: (%.0f, %.0f)", Input::GetMouseX(), Input::GetMouseY());


	if (ImGui::TreeNode("Window"))
	{
		const glm::vec2& windowPos = Game::Instance().GetWindow()->GetPos();
		ImGui::Text("Size: (%d, %d)", game.GetWindow()->GetWidth(), game.GetWindow()->GetHeight());
		ImGui::Text("Position: (%.0f, %.0f)", windowPos.x, windowPos.y);

		ImGui::TreePop();
	}
	ImGui::End();
	ImGui::SetNextWindowBgAlpha(1.0f);


	// From level
	auto& cameraController = scene->GetPrimaryCameraEntity().GetComponent<CameraComponent>().cameraController;

	ImGui::SetNextWindowBgAlpha(0.6f);
	ImGui::Begin("Level");

	ImGui::SeparatorText("Camera");
	if (ImGui::DragFloat2("pos##1", (float*)&cameraController->GetPosition()))
		cameraController->SetPosition(cameraController->GetPosition());
	if (ImGui::DragFloat("zoom", (float*)&cameraController->GetZoomLevel()))
		cameraController->SetZoomLevel(cameraController->GetZoomLevel());

	ImGui::Text("Bounds: (%.1f, %.1f)", cameraController->GetBounds().Right * 2, cameraController->GetBounds().Top * 2);

	glm::vec2 mousePos = scene->GetMousePosInScene();
	ImGui::Text("Mouse in world: (%.2f, %.2f)", mousePos.x, mousePos.y);

	ImGui::SeparatorText("Player");
	ImGui::DragFloat3("pos##2", (float*)&scene->GetPlayer()->GetComponent<TransformComponent>().position);
	ImGui::DragFloat("z", &scene->GetPlayer()->GetComponent<JumpComponent>().z);
	ImGui::DragFloat("velocity", &scene->GetPlayer()->GetComponent<JumpComponent>().velocity);
	ImGui::DragFloat("acceleration", &scene->GetPlayer()->GetComponent<JumpComponent>().acceleration);
	ImGui::Text("moving: %s", scene->GetPlayer()->GetComponent<MoveComponent>().isMoving() ? "Yes" : "No");

	ImGui::SeparatorText("Entity");
	ImGui::Text("Entity count: %d", scene->getRegistry().view<TransformComponent>().size());

	ImGui::End();
	ImGui::SetNextWindowBgAlpha(1.0f);

}

void GameLayer::OnRemove()
{
}