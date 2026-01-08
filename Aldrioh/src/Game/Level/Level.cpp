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

float zoomLevel = 10;

Level::Level(Scene& scene) : scene(scene), waveManager(scene, *this), collisionGrid(scene)
{
	// collisionDispatcher
	waveManager.Init();

	CollisionCallbackFunction callbackFireball = [](CollisionEvent& fireball, CollisionEvent& enemy) {
		fireball.e.QueueDestroy();
		HealthComponent& hc = enemy.e.GetComponent<HealthComponent>();
		hc.health -= 0.5f;
		auto& cesc = enemy.e.GetComponent<CoreEnemyStateComponent>();
		cesc.hitVisualTimer = 0.1;
		cesc.hitVisualState = HitVisualState::JUST_HIT;
		fireball.e.getScene()->CreateEntity("sound").AddComponent<SoundComponent>("bullet_impact");
		fireball.handled = true;
		};
	scene.GetCollisionDispatcher().AddCallback(EntityTypes::Fireball, EntityTypes::Enemy, callbackFireball);

	// Create player
	PlayerPrefab playerPrefab;
	Entity player = playerPrefab.create(scene);

	// Camera
	//FixedCameraPrefab fixedCameraPrefab;
	//fixedCameraPrefab.zoomLevel = zoomLevel;
	//camera = fixedCameraPrefab.create(scene);
	FollowingCameraPrefab followingCameraPrefab;
	followingCameraPrefab.zoomLevel = zoomLevel;
	followingCameraPrefab.entity = player;
	playerCamera = followingCameraPrefab.create(scene);


	// Add score entity
	Entity scoreEntity = scene.CreateEntityNoTransform("Score");
	scoreEntity.AddComponent<ScoreComponent>([this](float newScore)
		{
			this->UpdateScore(newScore);
		});

	collisionGrid.SetSize(100, 100);

}

Level::~Level()
{
}

static float elapsedTime = 0.0f;
static float asteroidSpawnSpeed = 0.1f;

void Level::OnUpdate(Timestep ts)
{
	waveManager.OnUpdate(ts);

	elapsedTime += ts;

	if (elapsedTime >= asteroidSpawnSpeed)
	{
		elapsedTime -= asteroidSpawnSpeed;

		AsteroidPrefab prefab;
		prefab.spawnPos = GenerateRandomSpawnCoords();
		prefab.speed = Math::Random::linearFloat(1.0f, 5.0f);
		prefab.angle = Math::degreesToRad(Math::Random::linearInt(0, 360));
		prefab.create(scene);
	}

}

void Level::OnRender(Timestep ts)
{
	Renderer::DrawQuad({ levelArea.bottomLeft, 0.5f }, Font::DEFAULT->GetCharSubTexture('a'), { 1, 1 }, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0, 1);
	Renderer::DrawQuad({ levelArea.topRight - glm::vec2{1,1}, 0.5f }, Font::DEFAULT->GetCharSubTexture('a'), { 1, 1 }, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0, 1);
}

LevelArea Level::GetScreenBorderOffsetByCamera(const glm::vec2& offset)
{
	auto& screenOffset = GetScreenBorderOffset();
	return { offset + screenOffset.bottomLeft, offset + screenOffset.topRight };
}

void Level::UpdateScore(float newScore)
{
	GlobalLayers::game->GetUIScoreText()->SetText("Score: " + std::to_string(int(newScore)));
}

// TODO See if can improve efficiency
glm::vec2 Level::GenerateRandomSpawnCoords()
{
	constexpr float SPAWN_OFFSET = -2;

	glm::vec2 spawnCoords{ 0 };

	glm::vec2 cameraPos = playerCamera.GetComponent<CameraComponent>().cameraController->GetPosition();
	LevelArea edgeBounds = GetScreenBorderOffsetByCamera(cameraPos);

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
