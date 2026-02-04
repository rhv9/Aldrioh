#include <pch.h>
#include "Level.h"
#include <Scene/Entity.h>
#include <Scene/Components.h>
#include <Game/SpriteCollection.h>
#include <Collision/Collision.h>
#include <Game/Components/ControllerComponents.h>
#include <Game.h>

#include <Graphics/Renderer.h>
#include <Graphics/RenderQueue.h>
#include <Game/RenderDepth.h>

#include <Math/Math.h>

#include <Game/Components/LevelComponents.h>
#include <Game/Components/EntityComponents.h>
#include <Game/GlobalLayers.h>

#include <Game/Entity/GameEntityPrefab.h>

#include <Game/Systems/RenderSystems.h>
#include <imgui.h>

#include <Game/Debug/GameDebugState.h>

float zoomLevel = 10;

ParticleTemplate particleTemplate_playerTakingDamage = []() {
	ParticleTemplate pt;
	pt.beginColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	pt.endColour = glm::vec4(0.8f, 0.8f, 1.0f, 0.9f);
	pt.beginSize = 0.35f;
	pt.endSize = 0.15f;
	pt.life = 0.2f;
	pt.velocity = { 0.0f, 0.0f };
	pt.velocityVariation = { 5.0f, 5.0f };
	pt.rotationRange = { Math::degreesToRad(-45), Math::degreesToRad(45) };
	pt.count = 4;
	return pt;
	}();

ParticleTemplate particleTemplate_fireballImpact = []() {
	ParticleTemplate pt;
	pt.beginColour = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);
	pt.endColour = glm::vec4(1.0f, 0.5f, 0.0f, 0.9f);
	pt.beginSize = 0.25f;
	pt.endSize = 0.05f;
	pt.life = 0.2f;
	pt.velocity = { 0.0f, 0.0f };
	pt.velocityVariation = { 5.0f, 5.0f };
	pt.rotationRange = { Math::degreesToRad(-45), Math::degreesToRad(45) };
	pt.count = 3;
	return pt;
	}();

auto OnDestroy_FireballImpact = [](Entity fireball) -> void {
	ParticleTemplate pt = particleTemplate_fireballImpact;
	pt.startPos = fireball.GetTransformComponent().position;
	fireball.getScene()->GetParticleManager().Emit(pt);
	};

Level::Level(Scene& scene) : scene(scene), waveManager(scene, *this), collectableManager(scene), playerStats(*this)
{
	waveManager.Init();
	scene.InitCollisionWorldSize(100, 100);
	collectableManager.Init(100, 100);

	scene.GetCollisionDispatcher().AddCallback(EntityTypes::Fireball, EntityTypes::Enemy, [](CollisionEvent& fireball, CollisionEvent& enemy)
		{
			fireball.e.AddComponent<OnDestroyComponent>(OnDestroy_FireballImpact);
			fireball.e.QueueDestroy();
			HealthComponent& hc = enemy.e.GetComponent<HealthComponent>();
			hc.health -= 0.5f;
			auto& cesc = enemy.e.GetComponent<CoreEnemyStateComponent>();
			cesc.hitVisualTimer = 0.1f;
			cesc.hitVisualState = HitVisualState::JUST_HIT;
			fireball.e.getScene()->CreateEntity("sound").AddComponent<SoundComponent>("bullet_impact");
			fireball.handled = true;
		});

	scene.GetCollisionDispatcher().AddCallback(EntityTypes::Fireball, EntityTypes::Asteroid, [](CollisionEvent& fireball, CollisionEvent& asteroid)
		{
			fireball.e.AddComponent<OnDestroyComponent>(OnDestroy_FireballImpact);
			fireball.e.QueueDestroy();
			HealthComponent& hc = asteroid.e.GetComponent<HealthComponent>();
			hc.health -= 0.5f;
			auto& cesc = asteroid.e.GetComponent<CoreEnemyStateComponent>();
			cesc.hitVisualTimer = 0.1f;
			cesc.hitVisualState = HitVisualState::JUST_HIT;
			fireball.e.getScene()->CreateEntity("sound").AddComponent<SoundComponent>("bullet_impact");
			fireball.handled = true;
		});

	scene.GetCollisionDispatcher().AddCallback(EntityTypes::Enemy, EntityTypes::Player, [](CollisionEvent& enemy, CollisionEvent& player)
		{
			if (false)
			{
				auto& hc = player.e.GetComponent<HealthComponent>();
				hc.health -= 0.03f;
				ParticleTemplate pt = particleTemplate_playerTakingDamage;
				pt.startPos = player.e.GetTransformComponent().position;
				player.e.getScene()->GetParticleManager().Emit(pt);
			}
		});

	scene.GetCollisionDispatcher().AddCallback(EntityTypes::Enemy, EntityTypes::Enemy, [](CollisionEvent& e1, CollisionEvent& e2)
		{

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
	levelTimeElapsed += ts;
	// TODO: does not necessarily have to update every tick.
	UpdateTimerText(levelTimeElapsed);
	UIProgressBar* uiPlayerHealthBar = GlobalLayers::game->GetUIHealthProgressBar();
	auto& hc = playerEntity.GetComponent<HealthComponent>();
	uiPlayerHealthBar->SetProgress(hc.health / hc.maxHealth);

	elapsedTime += ts;

	if (debugState.spawnEntites && elapsedTime >= asteroidSpawnSpeed)
	{
		elapsedTime -= asteroidSpawnSpeed;

		AsteroidPrefab prefab;
		prefab.spawnPos = GenerateRandomSpawnCoords();
		prefab.speed = Math::Random::linearFloat(1.0f, 5.0f);
		prefab.angle = Math::degreesToRad((float)Math::Random::linearInt(0, 360));
		prefab.create(scene);

		DroneEnemyPrefab dronePrefab;
		dronePrefab.maxHealth = 1.0f;
		dronePrefab.spawnPos = GenerateRandomSpawnCoords();
		dronePrefab.create(scene);
	}

}

glm::vec2 p0{ 0 }, p1{ 0, 1.0f }, p2{ 0 };
float tdelta = 0.1f;
bool renderBezierCurve = false;

void Level::OnRender(Timestep ts)
{
	glm::vec2 playerPos = EntitySystem::CalculateEntityTransformWithInterpolation(playerEntity, ts);

	// Update player health bar position
	UIProgressBar* uiPlayerHealthBar = GlobalLayers::game->GetUIHealthProgressBar();
	auto& playerCameraController = playerCamera.GetComponent<CameraComponent>().cameraController;
	glm::vec2 playerCameraPos = playerCameraController->GetPosition();
	glm::vec2 barPos = playerPos - playerCameraPos;
	barPos.y -= 0.7f;

	// TODO this does not need to be calculated every frame
	glm::vec2 uiArea = uiPlayerHealthBar->GetUIManager()->GetUIArea();
	glm::vec2 cameraArea = playerCameraController->GetBounds().GetSize();
	glm::vec2 cameraToUIMapping{ uiArea.x / cameraArea.x, uiArea.y / cameraArea.y };

	barPos.x *= cameraToUIMapping.x;
	barPos.y *= cameraToUIMapping.y;

	uiPlayerHealthBar->SetRelativePos(barPos);

	// Rendering jewels in the screen
	int startX = static_cast<int>(levelArea.bottomLeft.x + playerCameraPos.x - 1);
	int startY = static_cast<int>(levelArea.bottomLeft.y + playerCameraPos.y);
	int endX = static_cast<int>(levelArea.topRight.x + playerCameraPos.x + 2);
	int endY = static_cast<int>(levelArea.topRight.y + playerCameraPos.y + 1);

	//LOG_CORE_INFO("start: {},{}   end: {},{}", startX, startY, endX, endY);

	// Goes through each horizontal cell, then shift down and repeat for whole level area.
	for (int y = startY; y < endY; ++y)
	{
		for (int x = startX; x < endX; ++x)
		{
			CollectableMapping mapping = collectableManager.GetMapping({ x, y });
			CollectableCell& cell = collectableManager.GetChunk(mapping).GetCell(mapping);
			for (int i = 0; i < cell.count; ++i)
			{
				CellItem& cellData = cell.cellArray[i];
				CellItem::RenderData renderData = cellData.GetRenderData();

				glm::vec2 renderPos = glm::vec2{ x, y } + cellData.GetFloatOffset() - renderData.size / 2.0f;

				RenderQueue::EnQueue(RenderLayer::ZERO, glm::vec3{ renderPos, RenderDepth::COLLECTABLES }, renderData.spriteId, renderData.colour, renderData.size);
			}

			// Uncommenting this looks cool :D
			//RenderQueue::EnQueue(RenderLayer::ZERO, glm::vec3{ glm::vec2{x, y}, RenderDepth::COLLECTABLES }, Sprites::bullet_white, glm::vec4(1), { 0.5f, 0.5f });
		}
	}

	if (debugState.renderCollectableCells)
	{
		for (int y = startY; y < endY; ++y)
		{
			for (int x = startX; x < endX; ++x)
			{
				RenderQueue::EnQueue(RenderLayer::FOUR, { x, y, RenderDepth::COLLECTABLES }, Sprites::borderBox, Colour::GREEN);
			}
		}
		{
			auto& pcc = playerEntity.GetComponent<PlayerControllerComponent>();
			int startX = static_cast<int>(playerPos.x - pcc.radius);
			int startY = static_cast<int>(playerPos.y - pcc.radius);
			int endX = static_cast<int>(playerPos.x + pcc.radius);
			int endY = static_cast<int>(playerPos.y + pcc.radius);

			for (int y = startY; y < endY; ++y)
			{
				for (int x = startX; x < endX; ++x)
				{
					if (Math::dist(glm::vec2{ (float)x + 0.5f, (float)y + 0.5f }, playerPos) <= pcc.radius)
					{
						glm::vec2 chunkPos = { x, y };
						RenderQueue::EnQueue(RenderLayer::FOUR, glm::vec3{ chunkPos, RenderDepth::COLLECTABLES }, Sprites::borderBox, Colour::BLUE);
					}
				}
			}
		}
	}

	if (renderBezierCurve)
	{
		for (float t = 0; t < 1.0f; t += tdelta)
		{
			glm::vec2 size{ 0.09f };
			glm::vec2 p = Math::bezier3(p0, p1, p2, t) + playerPos - size / 2.0f;

			RenderQueue::EnQueue(RenderLayer::FOUR, { p, 1.0f }, Sprites::square, Colour::RED, size);
		}
	}
}

void Level::ImGuiLevelBar()
{
	if (ImGui::Checkbox("Spawn Enemies", &debugState.spawnEntites))
		elapsedTime = 0;

	ImGui::SeparatorText("Debugging");
	ImGui::Checkbox("Collectable Cells", &debugState.renderCollectableCells);
	ImGui::Checkbox("Collision World Visualisation", &GameDebugState::showCollisionWorldVisualisation);

	ImGui::Checkbox("Bezier tool", &renderBezierCurve);

	if (renderBezierCurve)
	{
		ImGui::DragFloat2("p0", (float*)&p0, 0.1f);
		ImGui::DragFloat2("p1", (float*)&p1, 0.1f);
		ImGui::DragFloat2("p2", (float*)&p2, 0.1f);
		ImGui::DragFloat("t delta", &tdelta, 0.02f, 0.02f, 1.0f);
	}


}

BoundingArea Level::GetScreenBorderOffsetByCamera(const glm::vec2& offset)
{
	auto& screenOffset = GetScreenBorderOffset();
	return { offset + screenOffset.bottomLeft, offset + screenOffset.topRight };
}

void Level::UpdateTimerText(float elapsedTime)
{
	int mins = static_cast<int>(elapsedTime / 60.0f);
	int seconds = static_cast<int>(elapsedTime) % 60;

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

void Level::OnLevelUp()
{
	GlobalLayers::game->GetUILevelCountText()->SetText(std::format("Level: {}", playerStats.GetLevelCount()));
}

void Level::OnExpGain()
{
	GlobalLayers::game->GetExpProgressBar()->SetProgress(playerStats.GetExpPercent());
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
