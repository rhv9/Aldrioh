#include <pch.h>
#include "GameEntityPrefab.h"

#include <Scene/Scene.h>
#include <Scene/Components.h>
#include <Components/Path/PathComponent.h>
#include <Components/Collision.h>
#include <Game/Components/LevelComponents.h>

#include <Scene/EntityCameraController.h>

#include <Game/SpriteCollection.h>
#include <Game.h>

#include <Math/Math.h>

static auto OnDestroy_AddScore = [](Entity e) {
	Entity scoreEntity = e.getScene()->CreateEntity("Add Score");
	scoreEntity.AddComponent<AddScoreComponent>(1.0f);
	};

Entity PlayerPrefab::create(Scene& scene)
{
	Entity player = scene.CreateEntity("Player");
	scene.SetPlayer(player);
	player.GetComponent<TransformComponent>().position = { startPos.x, startPos.y, 0.4f };
	VisualComponent& vc = player.AddComponent<VisualComponent>(Sprites::player_ship);
	vc.localTransform = { -0.5f, -0.5f, 0.0f };
	vc.colour = glm::vec4(0.5f, 0.5f, 1.0f, 1.0f);
	player.AddComponent<MoveComponent>(6.0f);
	player.AddComponent<EntityTypeComponent>(EntityTypes::Player);
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
	cameraController->SetPosition(position);
	// Add camera component
	Entity cameraEntity = scene.CreateEntityNoTransform("FixedCamera");
	cameraEntity.AddComponent<CameraComponent>(cameraController);

	return cameraEntity;
}


Entity FollowingCameraPrefab::create(Scene& scene)
{
	float aspectRatio = static_cast<float>(Game::Instance().GetWindow()->GetHeight()) / Game::Instance().GetWindow()->GetWidth();
	EntityCameraController* cameraController = new EntityCameraController(aspectRatio, 1.0f);
	cameraController->SetZoomLevel(zoomLevel);
	cameraController->SetEntity(entity);
	// Add camera component
	Entity cameraEntity = scene.CreateEntityNoTransform("EntityCamera");
	cameraEntity.AddComponent<CameraComponent>(cameraController);

	return cameraEntity;
}


Entity WobblyEnemyGroupPrefab::create(Scene& scene)
{
	Entity manager = scene.CreateEntity("Entity Manager");
	auto& emc = manager.AddComponent<EnemyManagerComponent>();

	glm::vec2 moveBy = Math::perpendicularClockwise(dirFacing);
	auto& mc = manager.AddComponent<MoveComponent>(speed);
	mc.updateMoveVec(moveBy);

	bool alreadyFlipped = false;
	emc.OnUpdateFunc = [distance = distance, alreadyFlipped, dirFacing = dirFacing * 1.0f](Timestep ts, Entity enemyManager) mutable
		{
			auto& tc = enemyManager.GetComponent<TransformComponent>();
			auto& mc = enemyManager.GetComponent<MoveComponent>();

			glm::vec2 move = mc.moveVec;
			glm::vec2 newPos = glm::vec2{ tc.position.x, tc.position.y };
			float length = glm::length(newPos);
			if (!alreadyFlipped && length > distance)
			{
				move = -move;
				alreadyFlipped = true;
			}
			if (length < distance)
				alreadyFlipped = false;

			move = glm::normalize(move + dirFacing);

			mc.updateMoveVec(move);
		};

	for (int y = 0; y < count.y; ++y)
	{
		for (int x = 0; x < count.x; ++x)
		{
			EnemyPrefab enemyPrefab;
			enemyPrefab.enemyManager = manager;
			enemyPrefab.maxHealth = 1;
			enemyPrefab.dirFacing = dirFacing;
			enemyPrefab.spawnPos = { startPos.x + x * spacing.x, startPos.y + y * spacing.y};
			enemyPrefab.create(scene);
		}
	}

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
	vc.rotation = Math::angle(dirFacing);
	enemy.AddComponent<MoveComponent>(1.0f);
	enemy.AddComponent<EntityTypeComponent>(EntityTypes::Enemy);
	enemy.AddComponent<AnimatedMovementComponent>(Sprites::animPlayerUp, Sprites::animPlayerDown, Sprites::animPlayerLeft, Sprites::animPlayerRight, 0.1f);
	enemy.AddComponent<CollisionBox>(glm::vec3{ -0.5f, -0.5f, 0.0f }, glm::vec2{ 1.0f, 1.0f });
	auto& dac = enemy.AddComponent<GlobalDumbAIComponent>();
	dac.enemyManager = enemyManager;
	enemy.AddComponent<HealthComponent>(maxHealth);
	enemy.AddComponent<OnDestroyComponent>(OnDestroy_AddScore);
	enemy.AddComponent<CoreEnemyStateComponent>();
	return enemy;
}

Entity EnemyPathPrefab::create(Scene& scene)
{
	if (points.size() < 2)
		return Entity{};

	const glm::vec2& spawnPos = points[0];
	Entity enemy = scene.CreateEntity("Enemy Path");
	auto& tc = enemy.GetComponent<TransformComponent>();
	tc.position = glm::vec3{ spawnPos, 0.4f };
	VisualComponent& vc = enemy.AddComponent<VisualComponent>(Sprites::player_ship);
	vc.localTransform = { -0.5f, -0.5f, 0.0f };
	vc.colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	vc.rotation = Math::angle(dirFacing);
	enemy.AddComponent<EntityTypeComponent>(EntityTypes::Enemy);
	enemy.AddComponent<AnimatedMovementComponent>(Sprites::animPlayerUp, Sprites::animPlayerDown, Sprites::animPlayerLeft, Sprites::animPlayerRight, 0.1f);
	enemy.AddComponent<CollisionBox>(glm::vec3{ -0.5f, -0.5f, 0.0f }, glm::vec2{ 1.0f, 1.0f });
	enemy.AddComponent<HealthComponent>(maxHealth);
	enemy.AddComponent<OnDestroyComponent>(OnDestroy_AddScore);
	enemy.AddComponent<CoreEnemyStateComponent>();

	// Pathing
	PathComponent& pc = enemy.AddComponent<PathComponent>();
	pc.currentPosition = { tc.position.x, tc.position.y };
	pc.prevPosition = pc.currentPosition;
	pc.path.Init(points);
	for (int i = 0; i < pc.path.maxPaths; ++i)
	{
		pc.path.pathConfigs[i].speed = speed + i;
	}
	return enemy;
}

Entity AsteroidPrefab::create(Scene& scene)
{
	Entity asteroid = scene.CreateEntity("Asteroid");
	auto& tc = asteroid.GetComponent<TransformComponent>();
	tc.position = glm::vec3{ spawnPos, 0.4f };
	VisualComponent& vc = asteroid.AddComponent<VisualComponent>(Sprites::asteroid_small);
	vc.localTransform = { -0.5f, -0.5f, 0.0f };
	MoveComponent& mc = asteroid.AddComponent<MoveComponent>(speed);
	mc.moveVec = Math::angleToNormalizedVector(angle);
	int rand = Math::Random::linearInt(-45, 45);
	RotationComponent& rc = asteroid.AddComponent<RotationComponent>(Math::degreesToRad(rand));
	rc.skipTicks = 5;

	asteroid.AddComponent<EntityTypeComponent>(EntityTypes::Asteroid);
	asteroid.AddComponent<CollisionBox>(glm::vec3{ -0.5f, -0.5f, 0.0f }, glm::vec2{ 1.0f, 1.0f });
	asteroid.AddComponent<HealthComponent>(maxHealth);
	asteroid.AddComponent<OnDestroyComponent>(OnDestroy_AddScore);
	asteroid.AddComponent<CoreEnemyStateComponent>();
	return asteroid;
}
