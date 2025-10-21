#include <pch.h>
#include "Level1.h"
#include <Scene/Entity.h>
#include <Scene/Components.h>
#include <Game/SpriteCollection.h>
#include <Components/Collision.h>
#include <Game/Components/ControllerComponents.h>
#include <Game.h>

#include <Graphics/Renderer.h>

#include <Math/Math.h>

#include <Game/Components/LevelComponents.h>
#include <Game/GlobalLayers.h>

float zoomLevel = 10;

void AddEnemy(Scene& scene, Entity enemyManager, const glm::vec2& spawnPos)
{
	// Create enemy
	Entity enemy = scene.CreateEntity("Enemy");
	auto& tc = enemy.GetComponent<TransformComponent>();
	tc.position = glm::vec3{ spawnPos, 0.4f };
	VisualComponent& vc = enemy.AddComponent<VisualComponent>(Sprites::player_ship);
	vc.localTransform = { -0.5f, -0.5f, 0.0f };
	vc.colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	vc.rotation = Math::PI;
	enemy.AddComponent<MoveComponent>(1);
	enemy.AddComponent<EntityTypeComponent>(EntityType::Enemy);
	enemy.AddComponent<AnimatedMovementComponent>(Sprites::animPlayerUp, Sprites::animPlayerDown, Sprites::animPlayerLeft, Sprites::animPlayerRight, 0.1f);
	enemy.AddComponent<CollisionBox>(glm::vec3{ -0.5f, -0.5f, 0.0f }, glm::vec2{ 1.0f, 1.0f });
	auto& dac = enemy.AddComponent<GlobalDumbAIComponent>();
	dac.enemyManager = enemyManager;
	enemy.AddComponent<HealthComponent>(1.0f);
	enemy.AddComponent<OnDestroyComponent>([](Entity e) {
		Entity scoreEntity = e.getScene()->CreateEntity("Add Score");
		scoreEntity.AddComponent<AddScoreComponent>(1.0f);
		});
}

Entity AddEnemyManager(Scene& scene)
{
	Entity manager = scene.CreateEntity("Entity Manager");
	auto& emc = manager.AddComponent<EnemyManagerComponent>();
	emc.move = 1;
	emc.distance = 3;
	emc.speed = 15;
	manager.AddComponent<MoveComponent>(1);

	return manager;
}

Level1::Level1(Scene& scene) : Level(scene)
{
	// collisionDispatcher
	CollisionCallbackFunction callback = [](Entity& e1, Entity& e2) {
		LOG_CORE_INFO("{} collides {}", e1.GetComponent<NameComponent>().name, e2.GetComponent<NameComponent>().name);
		};
	scene.GetCollisionDispatcher().AddCallback(EntityType::Player, EntityType::Enemy, callback);

	CollisionCallbackFunction callbackFireball = [](Entity& fireball, Entity& enemy) {
		fireball.Destroy();
		HealthComponent& hc = enemy.GetComponent<HealthComponent>();
		hc.health -= 0.5f;
		};
	scene.GetCollisionDispatcher().AddCallback(EntityType::Fireball, EntityType::Enemy, callbackFireball);

	// Create player
	Entity player = scene.CreateEntity("Player");
	scene.SetPlayer(player);
	player.GetComponent<TransformComponent>().position = { 0.0f, -zoomLevel / 2.0f, 0.4f };
	VisualComponent& vc = player.AddComponent<VisualComponent>(Sprites::player_ship);
	vc.localTransform = { -0.5f, -0.5f, 0.0f };
	vc.colour = glm::vec4(0.5f, 0.5f, 1.0f, 1.0f);
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

	Entity enemyManager = AddEnemyManager(scene);
	for (float y = 1; y < 8; y+=1.3f)
	{
		for (float x = -8; x < 8; x+= 2.1f)
		{
			AddEnemy(scene, enemyManager, { x, y });
		}
	}

	// Add score entity
	Entity scoreEntity = scene.CreateEntityNoTransform("Score");
	scoreEntity.AddComponent<ScoreComponent>([this](float newScore)
		{
			this->UpdateScore(newScore);
		});

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

void Level1::UpdateScore(float newScore)
{
	GlobalLayers::game->GetUIScoreText()->SetText("Score: " + std::to_string(int(newScore)));
}
