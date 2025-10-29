#pragma once

#include <Scene/EntityPrefab.h>
#include <Game/Components/EntityComponents.h>
#include <Game/Components/ControllerComponents.h>

struct PlayerPrefab : public EntityPrefab
{
	glm::vec2 startPos { 0 };
	DIR dir {DIRLOCK_FREE};
	EntityPrefabCreateOverride;
};

struct FixedCameraPrefab : public EntityPrefab
{
	float zoomLevel;
	EntityPrefabCreateOverride;
};

struct EnemyManagerPrefab : public EntityPrefab
{
	EntityPrefabCreateOverride;
};

struct EnemyPrefab : public EntityPrefab
{
	glm::vec2 spawnPos{ 0 };
	float maxHealth = 1;
	Entity enemyManager;
	EntityPrefabCreateOverride;
};

