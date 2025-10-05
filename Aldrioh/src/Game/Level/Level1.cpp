#include <pch.h>
#include "Level1.h"
#include <Scene/Entity.h>
#include <Scene/Components.h>
#include <Game/SpriteCollection.h>
#include <Components/Collision.h>
#include <Game/Components/ControllerComponents.h>
#include <Game.h>

#include <Graphics/Renderer.h>

float zoomLevel = 10;

Level1::Level1(Scene& scene) : Level(scene)
{
	// collisionDispatcher
	CollisionCallbackFunction callback = [](Entity& e1, Entity& e2) {
		LOG_CORE_INFO("{} collides {}", e1.GetComponent<NameComponent>().name, e2.GetComponent<NameComponent>().name);
		};
	scene.GetCollisionDispatcher().AddCallback(EntityType::Player, EntityType::Enemy, callback);

	CollisionCallbackFunction callbackFireball = [](Entity& e1, Entity& e2) {
		e1.Destroy();
		e2.Destroy();
		};
	scene.GetCollisionDispatcher().AddCallback(EntityType::Fireball, EntityType::Enemy, callbackFireball);

	// Create player
	Entity player = scene.CreateEntity("Player");
	scene.SetPlayer(player);
	player.GetComponent<TransformComponent>().position = { 0.0f, -zoomLevel / 2.0f, 0.4f };
	VisualComponent& vc = player.AddComponent<VisualComponent>(Sprites::player_ship);
	vc.localTransform = { -0.5f, -0.5f, 0.0f };
	vc.colour = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
	player.AddComponent<MoveComponent>(6.0f);
	player.AddComponent<EntityTypeComponent>(EntityType::Player);
	player.AddComponent<AnimatedMovementComponent>(Sprites::animPlayerUp, Sprites::animPlayerDown, Sprites::animPlayerLeft, Sprites::animPlayerRight, 0.1f);
	player.AddComponent<CollisionBox>(glm::vec3{ -0.5f, -0.5f, 0.0f }, glm::vec2{ 1.0f, 1.0f });
	player.AddComponent<JumpComponent>();
	auto& pcc = player.AddComponent<PlayerControllerComponent>();
	pcc.dirLock = DIRLOCK_UP;

	// Camera
	float aspectRatio = static_cast<float>(Game::Instance().GetWindow()->GetHeight()) / Game::Instance().GetWindow()->GetWidth();
	CameraController* cameraController = new CameraController(aspectRatio, 1.0f);
	cameraController->SetZoomLevel(zoomLevel);
	cameraController->SetPosition({ 0, 0 });
	// Add camera component
	Entity cameraEntity = scene.CreateEntity("RoamAndEntityCamera");
	cameraEntity.AddComponent<CameraComponent>(cameraController);
	cameraEntity.RemoveComponent<TransformComponent>(); // TODO: Need to consider this pls


}

Level1::~Level1()
{
}

void Level1::OnUpdate(Timestep ts)
{
}

void Level1::OnRender(Timestep ts)
{
	Renderer::DrawQuad({ 0, 0, 0.5f }, Sprites::get(Sprites::box), { 1, 1 }, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
}
