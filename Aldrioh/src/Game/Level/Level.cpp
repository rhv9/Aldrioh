#include <pch.h>
#include "Level.h"
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

Level::Level(Scene& scene) : scene(scene), waveManager(scene, *this)
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
	scene.GetCollisionDispatcher().AddCallback(EntityType::Fireball, EntityType::Enemy, callbackFireball);

	// Create player
	PlayerPrefab playerPrefab;
	playerPrefab.create(scene);

	// Camera
	FixedCameraPrefab fixedCameraPrefab;
	fixedCameraPrefab.zoomLevel = zoomLevel;
	camera = fixedCameraPrefab.create(scene);


	// Add score entity
	Entity scoreEntity = scene.CreateEntityNoTransform("Score");
	scoreEntity.AddComponent<ScoreComponent>([this](float newScore)
		{
			this->UpdateScore(newScore);
		});

}

Level::~Level()
{
}

void Level::OnUpdate(Timestep ts)
{
	waveManager.OnUpdate(ts);
}

void Level::OnRender(Timestep ts)
{
	Renderer::DrawQuad({ levelArea.bottomLeft, 0.5f }, Font::DEFAULT->GetCharSubTexture('a'), { 1, 1 }, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0, 1);
	Renderer::DrawQuad({ levelArea.topRight - glm::vec2{1,1}, 0.5f }, Font::DEFAULT->GetCharSubTexture('a'), { 1, 1 }, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0, 1);
}

void Level::UpdateScore(float newScore)
{
	GlobalLayers::game->GetUIScoreText()->SetText("Score: " + std::to_string(int(newScore)));
}

void Level::UpdateLevelArea()
{
	if (camera.Valid())
	{
		auto& cc = camera.GetComponent<CameraComponent>();
		auto& bounds = cc.cameraController->GetBounds();

		levelArea.bottomLeft = glm::vec2{ bounds.Left, bounds.Bottom };
		levelArea.topRight = glm::vec2{ bounds.Right, bounds.Top };
	}
}
