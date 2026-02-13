#pragma once
#include <Scene/Scene.h>
#include <Scene/Entity.h>

#include <Game/Level/CollectableManager.h>

#include "PlayerStats.h"
#include <Core/Window.h>
#include "WaveManager.h"
#include "LevelStats.h"

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
	void ImGuiRender(Timestep delta);

	void UpdateLevelArea();
	// Gives the bottom left and top right offset for screen to world position
	const BoundingArea& GetScreenBorderOffset() const { return levelArea; }
	BoundingArea GetScreenBorderOffsetByCamera(const glm::vec2& offset);
	BoundingArea GetDeathArea();
	Timestep GetElapsedTime() const { return levelTimeElapsed; }

	// Algorithm to generate the spawn coords for enemies/asteroids
	glm::vec2 GenerateRandomSpawnCoords();

	Entity GetPlayerCamera() { return playerCamera; }
	Entity GetPlayer() { return playerEntity; }

	CollectableManager& GetCollectableManager() { return collectableManager; }
	WaveManager& GetWaveManager() { return waveManager; }
	LevelStats& GetLevelStats() { return levelStats; }
	PlayerStats& GetPlayerStats() { return playerStats; }

	void OnLevelUp();
	void OnExpGain();

	// debugging related
	void Debug_SetEnableDebugCamera(bool enable);
	void Debug_OnMouseButtonForSpawningEnemies(MouseButtonEventArg& e);

	Scene& scene;
protected:
	BoundingArea levelArea;
	Entity playerCamera, debugCamera;
	Entity playerEntity;

	CollectableManager collectableManager;
	WaveManager waveManager;
	PlayerStats playerStats;
	LevelStats levelStats;

	EventCallbackID<MouseButtonEventArg> mouseButtonCallbackID;

	float levelTimeElapsed = 0.0f;
};

