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
#include <Game/Components/EntityComponents.h>
#include <Game/GlobalLayers.h>

#include <Game/Entity/GameEntityPrefab.h>

float zoomLevel = 10;

Level1::Level1(Scene& scene) : Level(scene)
{
	// collisionDispatcher

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
	scene.GetCollisionDispatcher().AddCallback(EntityType::Fireball, EntityType::Enemy, callbackFireball);

	// Create player
	PlayerPrefab playerPrefab;
	playerPrefab.create(scene);

	// Camera
	FixedCameraPrefab fixedCameraPrefab;
	fixedCameraPrefab.zoomLevel = zoomLevel;
	fixedCameraPrefab.create(scene);

	EnemyManagerPrefab enemyManagerPrefab;
	Entity enemyManager = enemyManagerPrefab.create(scene);
	for (float y = 1; y < 8; y += 1.3f)
	{
		for (float x = -8; x < 8; x += 2.1f)
		{
			EnemyPrefab enemyPrefab;
			enemyPrefab.enemyManager = enemyManager;
			enemyPrefab.maxHealth = 200;
			enemyPrefab.spawnPos = { x, y };
			enemyPrefab.create(scene);
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
	Renderer::DrawQuad({ 0, 0, 0.5f }, Font::DEFAULT->GetCharSubTexture('a'), { 1, 1 }, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0, 1);
}

void Level1::UpdateScore(float newScore)
{
	GlobalLayers::game->GetUIScoreText()->SetText("Score: " + std::to_string(int(newScore)));
}
