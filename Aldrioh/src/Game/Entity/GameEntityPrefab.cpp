#include <pch.h>
#include "GameEntityPrefab.h"

#include <Scene/Scene.h>
#include <Scene/Components.h>
#include <Components/Path/PathComponent.h>
#include <Collision/Collision.h>
#include <Game/Components/LevelComponents.h>

#include <Scene/EntityCameraController.h>
#include <FreeCameraController.h>

#include <Game/SpriteCollection.h>
#include <Game.h>
#include <Game/GlobalLayers.h>

#include <Math/Math.h>

ParticleTemplate particleTemplate_asteroidDestroyed = []() {
	ParticleTemplate pt;
	pt.beginColour = glm::vec4(0.25f, 0.25f, 0.25f, 1.0f);
	pt.endColour = glm::vec4(0.25f, 0.25f, 0.25f, 0.2f);
	pt.beginSize = 0.3f;
	pt.endSize = 0.3f;
	pt.life = 1.1f;
	pt.velocity = { 0.0f, 0.0f };
	pt.velocityVariation = { 2.0f, 2.0f };
	pt.rotationRange = { Math::degreesToRad(-100), Math::degreesToRad(100) };
	pt.count = 10;
	return pt;
	}();

auto OnDestroy_Player = [](Entity player) -> void {
	GlobalLayers::game->QueueTransitionTo(GlobalLayers::gameOver);
	};

auto OnDestroy_AsteroidParticles = [](Entity e) -> void {
	if (e.GetComponent<HealthComponent>().health <= 0.0f)
	{
		ParticleTemplate pt = particleTemplate_asteroidDestroyed;
		pt.startPos = e.GetTransformComponent().position;
		e.getScene()->GetParticleManager().Emit(pt);
	}
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
	player.AddComponent<CollisionBox>(glm::vec3{ -0.5f, -0.5f, 0.0f }, glm::vec2{ 1.0f, 1.0f });
	auto& pcc = player.AddComponent<PlayerControllerComponent>();
	pcc.dirLock = dir;
	player.AddComponent<HealthComponent>(maxHealth);
	player.AddComponent<OnDestroyComponent>(OnDestroy_Player);

	return player;
}

Entity FixedCameraPrefab::create(Scene& scene)
{
	float aspectRatio = static_cast<float>(Game::Instance().GetWindow()->GetHeight()) / Game::Instance().GetWindow()->GetWidth();
	Entity cameraEntity = scene.CreateEntityNoTransform("FixedCamera");
	CameraComponent& cc = cameraEntity.AddComponent<CameraComponent>(std::make_unique<CameraController>(aspectRatio, 1.0f));
	cc.cameraController->SetZoomLevel(zoomLevel);
	cc.cameraController->SetPosition(position);

	return cameraEntity;
}


Entity FollowingCameraPrefab::create(Scene& scene)
{
	float aspectRatio = static_cast<float>(Game::Instance().GetWindow()->GetHeight()) / Game::Instance().GetWindow()->GetWidth();
	// Add camera component
	Entity cameraEntity = scene.CreateEntityNoTransform("EntityCamera");
	CameraComponent& cc = cameraEntity.AddComponent<CameraComponent>(std::make_unique<EntityCameraController>(aspectRatio, 1.0f));
	EntityCameraController* cameraController = static_cast<EntityCameraController*>(cc.cameraController.get());

	cameraController->SetZoomLevel(zoomLevel);
	cameraController->SetEntity(entity);
	cameraController->ResetToPosition(startPos);
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
			enemyPrefab.spawnPos = { startPos.x + x * spacing.x, startPos.y + y * spacing.y };
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
	asteroid.AddComponent<CoreEnemyStateComponent>();
	asteroid.AddComponent<OnDestroyComponent>(OnDestroy_AsteroidParticles);
	return asteroid;
}

Entity FreeRoamCameraPrefab::create(Scene& scene)
{
	float aspectRatio = static_cast<float>(Game::Instance().GetWindow()->GetHeight()) / Game::Instance().GetWindow()->GetWidth();
	// Add camera component
	Entity cameraEntity = scene.CreateEntityNoTransform("Debugging Camera");
	CameraComponent& cc = cameraEntity.AddComponent<CameraComponent>(std::make_unique<FreeCameraController>(aspectRatio, 1.0f));
	static_cast<FreeCameraController*>(cc.cameraController.get())->SetSpeed(speed);
	cc.cameraController->SetZoomLevel(zoomLevel);
	return cameraEntity;
}

Entity DroneEnemyPrefab::create(Scene& scene)
{
	static ParticleTemplate particleTemplate_droneDestroyed = []() {
		ParticleTemplate pt;
		pt.beginColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		pt.endColour = glm::vec4(1.0f, 1.0f, 1.0f, 0.7f);
		pt.beginSize = 0.25f;
		pt.endSize = 0.25f;
		pt.life = 1.1f;
		pt.velocity = { 0.0f, 0.0f };
		pt.velocityVariation = { 2.0f, 2.0f };
		pt.rotationRange = { Math::degreesToRad(-100), Math::degreesToRad(100) };
		pt.count = 7;
		return pt;
		}();

	static ParticleTemplate particleTemplate_droneDestroyedRed = []() {
		ParticleTemplate pt = particleTemplate_droneDestroyed;
		pt.beginColour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		pt.endColour = glm::vec4(0.4f, 0.0f, 0.0f, 1.0f);
		pt.count = 1;
		return pt;
		}();

	static auto OnDestroy_DroneDeath = [](Entity e) -> void {
		if (e.GetComponent<HealthComponent>().health <= 0.0f)
		{
			glm::vec2 pos = e.GetTransformComponent().position;
			ParticleTemplate pt = particleTemplate_droneDestroyed;
			pt.startPos = pos;
			e.getScene()->GetParticleManager().Emit(pt);

			pt = particleTemplate_droneDestroyedRed;
			pt.startPos = pos;
			e.getScene()->GetParticleManager().Emit(pt);

			Level* level = e.getScene()->GetFirstEntity<LevelComponent>().GetComponent<LevelComponent>().level;
			CollectableMapping mapping = level->GetCollectableManager().GetMapping(pos);
			CollectableCell& cell = level->GetCollectableManager().GetChunk(mapping).GetCell(mapping);
			uint8_t xOffset = (pos.x - (int)pos.x) * CCellData::MAX_POINT_VALUE;
			uint8_t yOffset = (pos.y - (int)pos.y) * CCellData::MAX_POINT_VALUE;
			//LOG_CORE_INFO("Offset: {},{}", xOffset, yOffset);
			cell.AddCollectable(xOffset, yOffset, CollectableType::JEWEL1);
		}
		
		};

	Entity enemy = scene.CreateEntity("Drone");
	auto& tc = enemy.GetComponent<TransformComponent>();
	tc.position = glm::vec3{ spawnPos, 0.4f };
	VisualComponent& vc = enemy.AddComponent<VisualComponent>(Sprites::drone_mini);
	vc.localTransform = { -0.5f, -0.5f, 0.0f };
	enemy.AddComponent<MoveComponent>(speed);
	enemy.AddComponent<EntityTypeComponent>(EntityTypes::Enemy);
	glm::vec2 collisionSize{ 0.5f };
	enemy.AddComponent<CollisionBox>(glm::vec3{ collisionSize / -2.0f, 0.0f }, collisionSize);
	enemy.AddComponent<HealthComponent>(maxHealth);
	enemy.AddComponent<CoreEnemyStateComponent>();
	enemy.AddComponent<FollowPlayerAIComponent>();
	enemy.AddComponent<OnDestroyComponent>(OnDestroy_DroneDeath);

	return Entity();
}
