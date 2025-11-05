#include <pch.h>
#include "GameEntityPrefab.h"

#include <Scene/Scene.h>
#include <Scene/Components.h>
#include <Components/Collision.h>
#include <Game/Components/LevelComponents.h>

#include <Game/SpriteCollection.h>
#include <Game.h>

#include <Math/Math.h>

Entity PlayerPrefab::create(Scene& scene)
{
	Entity player = scene.CreateEntity("Player");
	scene.SetPlayer(player);
	player.GetComponent<TransformComponent>().position = { startPos.x, startPos.y, 0.4f };
	VisualComponent& vc = player.AddComponent<VisualComponent>(Sprites::player_ship);
	vc.localTransform = { -0.5f, -0.5f, 0.0f };
	vc.colour = glm::vec4(0.5f, 0.5f, 1.0f, 1.0f);
	player.AddComponent<MoveComponent>(6.0f);
	player.AddComponent<EntityTypeComponent>(EntityType::Player);
	player.AddComponent<AnimatedMovementComponent>(Sprites::animPlayerUp, Sprites::animPlayerDown, Sprites::animPlayerLeft, Sprites::animPlayerRight, 0.1f);
	player.AddComponent<CollisionBox>(glm::vec3{ -0.5f, -0.5f, 0.0f }, glm::vec2{ 1.0f, 1.0f });
	auto& pcc = player.AddComponent<PlayerControllerComponent>();
	pcc.dirLock = dir;

	return player;
}

Entity FixedCameraPrefab::create(Scene& scene)
{
	float aspectRatio = static_cast<float>(Game::Instance().GetWindow()->GetHeight()) / Game::Instance().GetWindow()->GetWidth();
	CameraController* cameraController = new CameraController(aspectRatio, 1.0f);
	cameraController->SetZoomLevel(zoomLevel);
	cameraController->SetPosition({ 0, 0 });
	// Add camera component
	Entity cameraEntity = scene.CreateEntityNoTransform("RoamAndEntityCamera");
	cameraEntity.AddComponent<CameraComponent>(cameraController);

	return cameraEntity;
}

Entity EnemyManagerPrefab::create(Scene& scene)
{
	Entity manager = scene.CreateEntity("Entity Manager");
	auto& emc = manager.AddComponent<EnemyManagerComponent>();

	float distance = 1;
	
	emc.OnUpdateFunc = [distance](Timestep ts, Entity enemyManager) mutable
		{
			auto& tc = enemyManager.GetComponent<TransformComponent>();
			auto& mc = enemyManager.GetComponent<MoveComponent>();

			glm::vec2 move = mc.moveVec;
			glm::vec2 newPos = glm::vec2{ tc.position.x, tc.position.y } + move;
			float length = glm::length(newPos);
			if (length > distance)
				move.x = -1;
			else if (length < -distance)
				move.x = 1;

			mc.updateMoveVec(move);
		};

	auto& mc = manager.AddComponent<MoveComponent>(speed);
	mc.updateMoveVec({ 1.0f, 0.0f });
	return manager;
}

Entity EnemyPrefab::create(Scene& scene)
{
	Entity enemy = scene.CreateEntity("Enemy");
	auto& tc = enemy.GetComponent<TransformComponent>();
	tc.position = glm::vec3{ spawnPos, 0.4f };
	VisualComponent& vc = enemy.AddComponent<VisualComponent>(Sprites::player_ship);
	vc.localTransform = { -0.5f, -0.5f, 0.0f };
	vc.colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	vc.rotation = Math::PI;
	enemy.AddComponent<MoveComponent>(1.0f);
	enemy.AddComponent<EntityTypeComponent>(EntityType::Enemy);
	enemy.AddComponent<AnimatedMovementComponent>(Sprites::animPlayerUp, Sprites::animPlayerDown, Sprites::animPlayerLeft, Sprites::animPlayerRight, 0.1f);
	enemy.AddComponent<CollisionBox>(glm::vec3{ -0.5f, -0.5f, 0.0f }, glm::vec2{ 1.0f, 1.0f });
	auto& dac = enemy.AddComponent<GlobalDumbAIComponent>();
	dac.enemyManager = enemyManager;
	enemy.AddComponent<HealthComponent>(maxHealth);
	enemy.AddComponent<OnDestroyComponent>([](Entity e) {
		Entity scoreEntity = e.getScene()->CreateEntity("Add Score");
		scoreEntity.AddComponent<AddScoreComponent>(1.0f);
		});
	enemy.AddComponent<CoreEnemyStateComponent>();
	return enemy;
}
