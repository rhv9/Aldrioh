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
#include <Game/Entity/GameEntities.h>

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
	GlobalLayers::game->OnPlayerDeath();
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
	player.GetComponent<TransformComponent>().UpdateBothPos(startPos);
	auto& vc = player.AddComponent<VisualComponent>(Sprites::player_ship);
	vc.localTransform = { -0.5f, -0.5f, 0.0f };
	vc.colour = glm::vec4(0.5f, 0.5f, 1.0f, 1.0f);
	auto& mcc = player.AddComponent<MoveControllerComponent>(10.0f);
	mcc.maxSpeed = 4.0f;
	mcc.falloffMultiplier = 1.0f;
	auto& pmc = player.AddComponent<PhysicsMovementComponent>();
	player.AddComponent<EntityTypeComponent>(EntityTypes::Player->entityId);
	player.AddComponent<CollisionComponent>(glm::vec3{ -0.5f, -0.5f, 0.0f }, glm::vec2{ 1.0f, 1.0f }, true);
	auto& pcc = player.AddComponent<PlayerControllerComponent>();
	pcc.dirLock = dir;
	
	StatModifier basicClass;
	basicClass.hp_base = 100;
	basicClass.dmg_base = 12;
	basicClass.critChance_base = 5;
	basicClass.critDmg_base = 100;
	basicClass.cooldown_base = 100;

	auto& sc = player.AddComponent<StatComponent>();
	sc.dirty = true;
	sc.baseStat = basicClass;

	player.AddComponent<HealthComponent>(basicClass.hp_base);
	player.AddComponent<OnDestroyComponent>(OnDestroy_Player);
	auto& msc = player.AddComponent<ModularShipComponent>();
	msc.bsiMax = 5;
	msc.smiMax = 3;
	msc.siMax = 4;
	msc.AddItem(ItemTypes::ShipModule_Shooter);

	player.AddComponent<ActionComponent>();

	return player;
}

Entity FixedCameraPrefab::create(Scene& scene)
{
	float aspectRatio = Game::Instance().GetWindow()->GetWidth() / static_cast<float>(Game::Instance().GetWindow()->GetHeight());
	Entity cameraEntity = scene.CreateEntityNoTransform("FixedCamera");
	CameraComponent& cc = cameraEntity.AddComponent<CameraComponent>(std::make_unique<CameraController>(aspectRatio, 1.0f));
	cc.cameraController->SetZoomLevel(zoomLevel);
	cc.cameraController->SetPosition(position);

	return cameraEntity;
}


Entity FollowingCameraPrefab::create(Scene& scene)
{
	float aspectRatio =  Game::Instance().GetWindow()->GetWidth() / static_cast<float>(Game::Instance().GetWindow()->GetHeight());
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
	auto& mcc = manager.AddComponent<MoveControllerComponent>(speed);
	mcc.moveDir = moveBy;

	bool alreadyFlipped = false;
	emc.OnUpdateFunc = [distance = distance, alreadyFlipped, dirFacing = dirFacing * 1.0f](Timestep ts, Entity enemyManager) mutable
		{
			auto& tc = enemyManager.GetComponent<TransformComponent>();
			auto& mc = enemyManager.GetComponent<MoveControllerComponent>();

			glm::vec2 move = mc.moveDir;
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

			mc.moveDir = move;
		};

	for (int y = 0; y < count.y; ++y)
	{
		for (int x = 0; x < count.x; ++x)
		{
			//EnemyPrefab enemyPrefab;
			//enemyPrefab.enemyManager = manager;
			//enemyPrefab.maxHealth = 1;
			//enemyPrefab.dirFacing = dirFacing;
			//enemyPrefab.spawnPos = { startPos.x + x * spacing.x, startPos.y + y * spacing.y };
			//enemyPrefab.create(scene);
		}
	}

	return manager;
}


Entity EnemyPathPrefab::create(Scene& scene)
{
	if (points.size() < 2)
		return Entity{};

	const glm::vec2& spawnPos = points[0];
	Entity enemy = scene.CreateEntity("Enemy Path");
	auto& tc = enemy.GetComponent<TransformComponent>();
	tc.UpdateBothPos(spawnPos);
	VisualComponent& vc = enemy.AddComponent<VisualComponent>(Sprites::player_ship);
	vc.localTransform = { -0.5f, -0.5f, 0.0f };
	vc.colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	vc.rotation = Math::angle(dirFacing);
	enemy.AddComponent<EntityTypeComponent>(EnemyEntityTypes::Enemy->entityId);
	enemy.AddComponent<AnimatedMovementComponent>(Sprites::animPlayerUp, Sprites::animPlayerDown, Sprites::animPlayerLeft, Sprites::animPlayerRight, 0.1f);
	enemy.AddComponent<CollisionComponent>(glm::vec3{ -0.5f, -0.5f, 0.0f }, glm::vec2{ 1.0f, 1.0f });
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
	tc.UpdateBothPos(spawnPos);
	VisualComponent& vc = asteroid.AddComponent<VisualComponent>(Sprites::asteroid_small);
	vc.localTransform = { -0.5f, -0.5f, 0.0f };
	asteroid.AddComponent<PhysicsMovementComponent>(false).resultantVelocity = Math::angleToNormalizedVector(angle);
	int rand = Math::Random::linearInt(-45, 45);
	RotationComponent& rc = asteroid.AddComponent<RotationComponent>(Math::degreesToRad(static_cast<float>(rand)));
	rc.skipTicks = 5;

	asteroid.AddComponent<EntityTypeComponent>(EnemyEntityTypes::Asteroid->entityId);
	asteroid.AddComponent<CollisionComponent>(glm::vec3{ -0.5f, -0.5f, 0.0f }, glm::vec2{ 1.0f, 1.0f });
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
	static_cast<FreeCameraController*>(cc.cameraController.get())->SetEnabledWASDMovement(false);

	cc.cameraController->SetZoomLevel(zoomLevel);
	return cameraEntity;
}

