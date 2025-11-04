#pragma once

#include <Scene/EntityPrefab.h>
#include <Game/Components/EntityComponents.h>
#include <Game/Components/ControllerComponents.h>

struct PlayerPrefab : public EntityPrefab
{
	glm::vec2 startPos { 0 };
	DIR dir {DIRLOCK_FREE};
	virtual Entity create(Scene& scene) override;
};

struct FixedCameraPrefab : public EntityPrefab
{
	float zoomLevel;
	virtual Entity create(Scene& scene) override;
};

struct EnemyManagerPrefab : public EntityPrefab
{
	virtual Entity create(Scene& scene) override;
};

struct EnemyPrefab : public EntityPrefab
{
	glm::vec2 spawnPos{ 0 };
	float maxHealth = 1;
	Entity enemyManager;
	virtual Entity create(Scene& scene) override;
};

