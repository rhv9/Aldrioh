#include <pch.h>
#include "Level.h"
#include <Scene/Entity.h>
#include <Scene/Components.h>
#include <Game/SpriteCollection.h>
#include <Collision/Collision.h>
#include <Game/Components/ControllerComponents.h>
#include <Game.h>

#include <Graphics/Renderer.h>

#include <Math/Math.h>

#include <Game/Components/LevelComponents.h>
#include <Game/Components/EntityComponents.h>
#include <Game/GlobalLayers.h>

#include <Game/Entity/GameEntityPrefab.h>

#include <Game/Systems/RenderSystems.h>

float zoomLevel = 10;

Level::Level(Scene& scene) : scene(scene), waveManager(scene, *this)
{
	// collisionDispatcher
	waveManager.Init();

	scene.GetCollisionDispatcher().AddCallback(EntityTypes::Fireball, EntityTypes::Enemy, [](CollisionEvent& fireball, CollisionEvent& enemy)
		{
			fireball.e.QueueDestroy();
			HealthComponent& hc = enemy.e.GetComponent<HealthComponent>();
			hc.health -= 0.5f;
			auto& cesc = enemy.e.GetComponent<CoreEnemyStateComponent>();
			cesc.hitVisualTimer = 0.1;
			cesc.hitVisualState = HitVisualState::JUST_HIT;
			fireball.e.getScene()->CreateEntity("sound").AddComponent<SoundComponent>("bullet_impact");
			fireball.handled = true;
		});

	scene.GetCollisionDispatcher().AddCallback(EntityTypes::Fireball, EntityTypes::Asteroid, [](CollisionEvent& fireball, CollisionEvent& asteroid)
		{
			fireball.e.QueueDestroy();
			HealthComponent& hc = asteroid.e.GetComponent<HealthComponent>();
			hc.health -= 0.5f;
			auto& cesc = asteroid.e.GetComponent<CoreEnemyStateComponent>();
			cesc.hitVisualTimer = 0.1;
			cesc.hitVisualState = HitVisualState::JUST_HIT;
			fireball.e.getScene()->CreateEntity("sound").AddComponent<SoundComponent>("bullet_impact");
			fireball.handled = true;
		});

	scene.GetCollisionDispatcher().AddCallback(EntityTypes::Enemy, EntityTypes::Player, [](CollisionEvent& enemy, CollisionEvent& player)
		{
			auto& hc = player.e.GetComponent<HealthComponent>();
			hc.health -= 0.03f;
		});


	// Create player
	PlayerPrefab playerPrefab;
	playerPrefab.startPos = { 200, 200 };
	playerEntity = playerPrefab.create(scene);

	// Camera
	// Main player camera
	FollowingCameraPrefab followingCameraPrefab;
	followingCameraPrefab.zoomLevel = zoomLevel;
	followingCameraPrefab.entity = playerEntity;
	followingCameraPrefab.startPos = playerPrefab.startPos;
	playerCamera = followingCameraPrefab.create(scene);
	scene.SetPrimaryCameraEntity(playerCamera);

	// Debug free roam camera 
	FreeRoamCameraPrefab freeCameraPrefab;
	freeCameraPrefab.zoomLevel = zoomLevel + 2;
	freeCameraPrefab.speed = 0.05f;
	debugCamera = freeCameraPrefab.create(scene);

	// Add score entity
	Entity scoreEntity = scene.CreateEntityNoTransform("Score");
	scoreEntity.AddComponent<ScoreComponent>([this](float newScore)
		{
			this->UpdateScore(newScore);
		});
	LOG_CORE_INFO("Test {}", scene.GetCollisionWorld().GetMapping({ 15.0f, 16.0f }).ToString());
}

Level::~Level()
{
}

static float elapsedTime = 0.0f;
static float asteroidSpawnSpeed = 0.4f;

void Level::OnUpdate(Timestep ts)
{
	waveManager.OnUpdate(ts);

	// TODO: does not necessarily have to update every tick.
	UpdateTimerText(Platform::GetElapsedTime() - levelStartTime);
	UIProgressBar* uiPlayerHealthBar = GlobalLayers::game->GetUIHealthProgressBar();
	auto& hc = playerEntity.GetComponent<HealthComponent>();
	uiPlayerHealthBar->SetProgress(hc.health / hc.maxHealth);

	elapsedTime += ts;

	if (elapsedTime >= asteroidSpawnSpeed)
	{
		elapsedTime -= asteroidSpawnSpeed;

		AsteroidPrefab prefab;
		prefab.spawnPos = GenerateRandomSpawnCoords();
		prefab.speed = Math::Random::linearFloat(1.0f, 5.0f);
		prefab.angle = Math::degreesToRad(Math::Random::linearInt(0, 360));
		prefab.create(scene);

		DroneEnemyPrefab dronePrefab;
		dronePrefab.maxHealth = 1.0f;
		dronePrefab.spawnPos = GenerateRandomSpawnCoords();
		dronePrefab.create(scene);
	}

}

void Level::OnRender(Timestep ts)
{
	Renderer::DrawQuad({ levelArea.bottomLeft, 0.5f }, Font::DEFAULT->GetCharSubTexture('a'), { 1, 1 }, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0, 1);
	Renderer::DrawQuad({ levelArea.topRight - glm::vec2{1,1}, 0.5f }, Font::DEFAULT->GetCharSubTexture('a'), { 1, 1 }, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0, 1);

	glm::vec2 playerPos = EntitySystem::CalculateEntityTransformWithInterpolation(playerEntity, ts);

	// Update player health bar position
	UIProgressBar* uiPlayerHealthBar = GlobalLayers::game->GetUIHealthProgressBar();
	auto& playerCameraController = playerCamera.GetComponent<CameraComponent>().cameraController;
	glm::vec2 barPos = playerPos - playerCameraController->GetPosition();
	barPos.y -= 0.7f;

	// TODO this does not need to be calculated every frame
	glm::vec2 uiArea = uiPlayerHealthBar->GetUIManager()->GetUIArea();
	glm::vec2 cameraArea = playerCameraController->GetBounds().GetSize();
	glm::vec2 cameraToUIMapping{ uiArea.x / cameraArea.x, uiArea.y / cameraArea.y };

	barPos.x *= cameraToUIMapping.x;
	barPos.y *= cameraToUIMapping.y;

	uiPlayerHealthBar->SetRelativePos(barPos);
}

BoundingArea Level::GetScreenBorderOffsetByCamera(const glm::vec2& offset)
{
	auto& screenOffset = GetScreenBorderOffset();
	return { offset + screenOffset.bottomLeft, offset + screenOffset.topRight };
}

void Level::UpdateScore(float newScore)
{
	GlobalLayers::game->GetUIScoreText()->SetText("Score: " + std::to_string(int(newScore)));
}

void Level::UpdateTimerText(float elapsedTime)
{
	int mins = elapsedTime / 60.0f;
	int seconds = (int)elapsedTime % 60;

	std::string timerText = std::format("{}:{:02}", mins, seconds);
	GlobalLayers::game->GetUITimerText()->SetText(timerText);
}

// TODO See if can improve efficiency
glm::vec2 Level::GenerateRandomSpawnCoords()
{
	constexpr float SPAWN_OFFSET = 2;

	glm::vec2 spawnCoords{ 0 };

	glm::vec2 cameraPos = playerCamera.GetComponent<CameraComponent>().cameraController->GetPosition();
	BoundingArea edgeBounds = GetScreenBorderOffsetByCamera(cameraPos);

	glm::vec2 maxCoord{ edgeBounds.topRight.x + SPAWN_OFFSET, edgeBounds.topRight.y + SPAWN_OFFSET };
	glm::vec2 minCoord{ edgeBounds.bottomLeft.x - SPAWN_OFFSET, edgeBounds.bottomLeft.y - SPAWN_OFFSET };

	// Choose edge
	int edge = Math::Random::linearInt(0, 3);

	if (edge == 0)
	{
		// left edge
		spawnCoords.x = minCoord.x;
		spawnCoords.y = Math::Random::linearFloat(minCoord.y, maxCoord.y);
	}
	else if (edge == 1)
	{
		// top edge
		spawnCoords.x = Math::Random::linearFloat(minCoord.x, maxCoord.x);
		spawnCoords.y = maxCoord.y;
	}
	else if (edge == 2)
	{
		// right edge
		spawnCoords.x = maxCoord.x;
		spawnCoords.y = Math::Random::linearFloat(minCoord.y, maxCoord.y);
	}
	else
	{
		// bottom edge
		spawnCoords.x = Math::Random::linearFloat(minCoord.x, maxCoord.x);
		spawnCoords.y = minCoord.y;
	}

	return spawnCoords;
}

BoundingArea Level::GetDeathArea()
{
	CameraComponent& cameraComponent = GetPlayerCamera().GetComponent<CameraComponent>();
	const BoundingArea& offset = GetScreenBorderOffsetByCamera(cameraComponent.cameraController->GetPosition());

	glm::vec2 bottomLeft = offset.bottomLeft - 5.0f;
	glm::vec2 topRight = offset.topRight + 5.0f;

	BoundingArea boundingArea;
	boundingArea.bottomLeft = bottomLeft;
	boundingArea.topRight = topRight;
	return boundingArea;
}

void Level::SetEnableDebugCamera(bool enable)
{
	if (enable)
	{
		debugCamera.GetComponent<CameraComponent>().cameraController->SetPosition(playerCamera.GetComponent<CameraComponent>().cameraController->GetPosition());
		scene.SetPrimaryCameraEntity(debugCamera);
	}
	else
		scene.SetPrimaryCameraEntity(playerCamera);
}

void Level::UpdateLevelArea()
{
	if (playerCamera.IsValid())
	{
		auto& cc = playerCamera.GetComponent<CameraComponent>();
		auto& bounds = cc.cameraController->GetBounds();

		levelArea.bottomLeft = glm::vec2{ bounds.Left, bounds.Bottom };
		levelArea.topRight = glm::vec2{ bounds.Right, bounds.Top };
	}
}
