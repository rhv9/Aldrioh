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
#include <Game/Entity/GameEntities.h>

#include <Game/Systems/RenderSystems.h>
#include <imgui.h>

#include <Game/Debug/GameDebugState.h>
#include <Input/Input.h>

float zoomLevel = 10;

struct ImGuiSettings
{
	bool enabledImGui = false;

	bool shouldUpdateScene = true;
	bool shouldPathRecord = false;
	bool pathStartStopHovered = false;

	bool EnabledDebugCamera = false;
};
struct LevelEditorData
{
	std::vector<glm::vec2> points;
};

static LevelEditorData levelEditorData;
static ImGuiSettings imGuiSettings;

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

Level::Level(Scene& scene) : scene(scene), playerStats(*this), waveManager(scene, *this)
{
	GameDebugState::level_spawnEntites = false;

	scene.GetCollisionZone().Init(60, 40, 1);
	waveManager.InitWaveConfig();

	scene.GetCollisionDispatcher().AddCallback(EntityTypes::Fireball->entityId, EnemyEntityTypes::Enemy->entityId, [](CollisionEvent& fireball, CollisionEvent& enemy)
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

	scene.GetCollisionDispatcher().AddCallback(EntityTypes::Fireball->entityId, EnemyEntityTypes::Asteroid->entityId, [](CollisionEvent& fireball, CollisionEvent& asteroid)
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

	scene.GetCollisionDispatcher().AddCallback(EnemyEntityTypes::Enemy->entityId, EntityTypes::Player->entityId, [](CollisionEvent& enemy, CollisionEvent& player)
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

	scene.GetCollisionDispatcher().AddCallback(EnemyEntityTypes::Enemy->entityId, EnemyEntityTypes::Enemy->entityId, [](CollisionEvent& e1, CollisionEvent& e2)
		{
		});

	// Create player
	PlayerPrefab playerPrefab;
	playerPrefab.startPos = { 0, 0 };
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

	// Debugging
	mouseButtonCallbackID = Game::Instance().GetWindow()->MouseButtonEventHandler.RegisterCallback(EVENT_BIND_MEMBER_FUNCTION(Level::Debug_OnMouseButtonForSpawningEnemies));
}

Level::~Level()
{
}

void Level::OnUpdate(Timestep ts)
{
	if (GameDebugState::level_spawnEntites)
		levelTimeElapsed += ts;

	waveManager.OnUpdate(ts);

	glm::vec2 playerCameraPos = playerCamera.GetComponent<CameraComponent>().cameraController->GetPosition();
	CollectableMapping bottomLeftMapping = collectableManager.GetMapping(levelArea.bottomLeft + playerCameraPos);
	CollectableMapping topRightMapping = collectableManager.GetMapping(levelArea.topRight + playerCameraPos);

	collectableManager.OnUpdate(ts, bottomLeftMapping, topRightMapping);
}

glm::vec2 p0{ 0 }, p1{ 0, 1.0f }, p2{ 0 };
float tdelta = 0.1f;
bool renderBezierCurve = false;

void Level::OnRender(Timestep ts)
{
	glm::vec2 playerPos = EntitySystem::CalculateEntityTransformWithInterpolation(playerEntity, ts);
	glm::vec2 playerCameraPos = playerCamera.GetComponent<CameraComponent>().cameraController->GetPosition();

	CollectableMapping bottomLeftMapping = collectableManager.GetMapping(levelArea.bottomLeft + playerCameraPos);
	CollectableMapping topRightMapping = collectableManager.GetMapping(levelArea.topRight + playerCameraPos);

	collectableManager.RenderChunks(bottomLeftMapping, topRightMapping);
	collectableManager.Debug_Render(*this, ts, bottomLeftMapping, topRightMapping);

	if (GameDebugState::renderLevelArea)
	{
		glm::vec2 levelOffsetBottomLeft = levelArea.bottomLeft + playerCameraPos;
		glm::vec2 levelOffsetTopRight = levelArea.topRight + playerCameraPos;
		glm::vec2 size{ levelOffsetTopRight.x - levelOffsetBottomLeft.x, levelOffsetTopRight.y - levelOffsetBottomLeft.y };
		RenderQueue::EnQueue(RenderLayer::FOUR, glm::vec3{ levelOffsetBottomLeft, 0.8f }, Sprites::borderBox, Colour::RED, size);
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

	int i = 0;
	for (const glm::vec2& point : levelEditorData.points)
	{
		constexpr glm::vec2 size = { 0.45f, 0.45f };
		RenderQueue::EnQueue(RenderLayer::ONE, glm::vec3{ point - size / 2.0f, 1.0f }, Font::DEFAULT->GetCharSubTexture(i++ + '0'), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), size, 0, 1);
	}
}

void Level::Debug_OnMouseButtonForSpawningEnemies(MouseButtonEventArg& e)
{
	if (imGuiSettings.shouldPathRecord && !imGuiSettings.pathStartStopHovered)
	{
		if (e.IsReleased(Input::MOUSE_BUTTON_LEFT))
		{
			levelEditorData.points.push_back(scene.GetMousePosInScene());
		}
	}

	if (GameDebugState::clickToSpawnEnemies && e.IsPressed(Input::MOUSE_BUTTON_1))
	{
		DroneEnemyPrefab dronePrefab;
		dronePrefab.maxHealth = 1.0f;
		dronePrefab.spawnPos = scene.GetMousePosInScene();
		dronePrefab.create(scene);
	}
}

void Level::ImGuiLevelBar(Timestep delta)
{
	ImGui::Checkbox("Spawn Enemies", &GameDebugState::level_spawnEntites);

	ImGui::Checkbox("Mouse spawn enemies", &GameDebugState::clickToSpawnEnemies);

	ImGui::SeparatorText("Debugging");
	ImGui::Checkbox("Level Area", &GameDebugState::renderLevelArea);
	ImGui::Checkbox("Collectable Cells", &GameDebugState::renderCollectableCells);
	ImGui::Checkbox("Collectable Chunks", &GameDebugState::renderChunkBordersBeingRendered);
	ImGui::Checkbox("Loaded/Unloaded Chunks", &GameDebugState::showLoadedAndUnloadedCollectableChunks);

	ImGui::Checkbox("Collision World Visualisation", &GameDebugState::showCollisionZoneVisualisation);
	ImGui::Checkbox("Bezier tool", &renderBezierCurve);

	if (renderBezierCurve)
	{
		ImGui::DragFloat2("p0", (float*)&p0, 0.1f);
		ImGui::DragFloat2("p1", (float*)&p1, 0.1f);
		ImGui::DragFloat2("p2", (float*)&p2, 0.1f);
		ImGui::DragFloat("t delta", &tdelta, 0.02f, 0.02f, 1.0f);
	}

	if (ImGui::Checkbox("Debugging Camera", &GameDebugState::enabledDebugCamera))
		Debug_SetEnableDebugCamera(GameDebugState::enabledDebugCamera);

	if (ImGui::Button(GameDebugState::shouldUpdateScene ? "Pause" : "Play"))
		GameDebugState::shouldUpdateScene = !GameDebugState::shouldUpdateScene;

	ImGui::SeparatorText("Entity Pathing");

	if (ImGui::Button(imGuiSettings.shouldPathRecord ? "Stop recording path" : "Start recording path"))
		imGuiSettings.shouldPathRecord = !imGuiSettings.shouldPathRecord;

	if (ImGui::IsItemHovered())
		imGuiSettings.pathStartStopHovered = true;
	else
		imGuiSettings.pathStartStopHovered = false;

	if (ImGui::Button("Reset Path"))
		levelEditorData.points.clear();

	static float pathEnemySpeed = 1.0f;
	ImGui::DragFloat("Initial Speed", &pathEnemySpeed);


	if (ImGui::Button("Create enemy"))
	{
		LOG_INFO("Creating a path enemy!");
		EnemyPathPrefab epp;
		epp.points = levelEditorData.points;
		epp.speed = pathEnemySpeed;
		epp.create(scene);
	}

	static bool keepCreating = false;
	static float interval = 1.0f;
	static float elapsedTime = 0.0f;
	elapsedTime += delta;

	ImGui::DragFloat("Interval", &interval);

	if (ImGui::Button(keepCreating ? "Stop  da centipede" : "Begin epicness"))
		keepCreating = !keepCreating;

	if (elapsedTime >= interval)
	{
		elapsedTime -= interval;
		if (keepCreating)
		{
			EnemyPathPrefab epp;
			epp.points = levelEditorData.points;
			epp.speed = pathEnemySpeed;
			epp.create(scene);
		}
	}

	if (levelEditorData.points.size() != 0 || imGuiSettings.shouldPathRecord)
	{
		std::string text;
		text.reserve(512);

		for (const glm::vec2& point : levelEditorData.points)
		{
			text.append(std::format(",({:.1f},{:.1f})", point.x, point.y));
		}

		ImGui::Text(std::format("Path: {}", text).c_str());
	}
}

BoundingArea Level::GetScreenBorderOffsetByCamera(const glm::vec2& offset)
{
	auto& screenOffset = GetScreenBorderOffset();
	return { offset + screenOffset.bottomLeft, offset + screenOffset.topRight };
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
	GlobalLayers::game->GetUILayer()->GetUILevelCountText()->SetText(std::format("Level: {}", playerStats.GetLevelCount()));
}

void Level::OnExpGain()
{
	GlobalLayers::game->GetUILayer()->GetExpProgressBar()->SetProgress(playerStats.GetExpPercent());
}

void Level::Debug_SetEnableDebugCamera(bool enable)
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
