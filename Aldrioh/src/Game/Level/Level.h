#pragma once
#include <Scene/Scene.h>
#include <Scene/Entity.h>

#include <Game/Waves/WaveManager.h>
#include <Game/Level/CollectableManager.h>

#include "PlayerStats.h"

struct BoundingArea
{
	glm::vec2 bottomLeft{ 0 };
	glm::vec2 topRight{ 0 };

	float GetWidth() { return topRight.x - bottomLeft.x; }
	float GetHeight() { return topRight.y - bottomLeft.y; }
};

class Level
{
public:

	Level(Scene& scene);
	~Level();
	void OnUpdate(Timestep ts);
	void OnRender(Timestep ts);

	void ImGuiLevelBar();

	void UpdateLevelArea();
	// Gives the bottom left and top right offset for screen to world position
	const BoundingArea& GetScreenBorderOffset() const { return levelArea; }
	BoundingArea GetScreenBorderOffsetByCamera(const glm::vec2& offset);

	void UpdateTimerText(float elapsedTime);

	// Algorithm to generate the spawn coords for enemies/asteroids
	glm::vec2 GenerateRandomSpawnCoords();

	Entity GetPlayerCamera() { return playerCamera; }
	Entity GetPlayer() { return playerEntity; }

	BoundingArea GetDeathArea();

	CollectableManager& GetCollectableManager() { return collectableManager; }

	PlayerStats& GetPlayerStats() { return playerStats; }
	void OnLevelUp();
	void OnExpGain();

	Scene& scene;

	// debugging related
	void SetEnableDebugCamera(bool enable);
protected:
	BoundingArea levelArea;
	Entity playerCamera, debugCamera;
	Entity playerEntity;

	WaveManager waveManager;
	CollectableManager collectableManager;
	PlayerStats playerStats;

	float levelTimeElapsed = 0.0f;
};

