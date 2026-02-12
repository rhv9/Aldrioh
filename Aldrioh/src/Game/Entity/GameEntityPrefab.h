#pragma once

#include <Scene/EntityPrefab.h>
#include <Game/Components/EntityComponents.h>
#include <Game/Components/ControllerComponents.h>
#include "EntityType.h"

struct PlayerPrefab : public EntityPrefab
{
	glm::vec2 startPos { 0 };
	DIR dir {DIRLOCK_FREE};
	float maxHealth = 10.0f;
	virtual Entity create(Scene& scene) override;
};

struct FixedCameraPrefab : public EntityPrefab
{
	float zoomLevel = 1.0f;
	glm::vec2 position{ 0 };
	virtual Entity create(Scene& scene) override;
};

struct FollowingCameraPrefab : public EntityPrefab
{
	float zoomLevel;
	Entity entity;
	glm::vec2 startPos{ 0 };
	virtual Entity create(Scene& scene) override;
};

struct FreeRoamCameraPrefab : public EntityPrefab
{
	float zoomLevel;
	float speed = 1.0f;
	virtual Entity create(Scene& scene) override;
};

struct WobblyEnemyGroupPrefab : public EntityPrefab
{
	float speed = 4.0f;
	glm::vec2 dirFacing{ 0, -1 };
	float dirFacingMovingSpeed = 0.0f;
	float distance = 1;

	glm::vec2 spacing{ 1 };
	glm::vec2 count{ 1, 1 };

	glm::vec2 startPos{ 0, 0 };

	virtual Entity create(Scene& scene) override;
};

struct EnemyPrefab : public EntityPrefab
{
	glm::vec2 spawnPos{ 0 };
	float maxHealth = 1;
	Entity enemyManager;
	glm::vec2 dirFacing{ 0, -1 };
	virtual Entity create(Scene& scene) override;
};

struct EnemyPathPrefab : public EntityPrefab
{
	float maxHealth = 1;
	glm::vec2 dirFacing{ 0, -1 };
	std::vector<glm::vec2> points;
	float speed = 1.0f;

	virtual Entity create(Scene& scene) override;
};

struct DroneEnemyPrefab : public EntityPrefab
{
	glm::vec2 spawnPos{ 0 };
	float maxHealth = 1;
	float speed = 1.0f;
	virtual Entity create(Scene& scene) override;
};

struct AsteroidPrefab : public EntityPrefab
{
	float maxHealth = 1;
	float speed = 1.0f;
	glm::vec2 spawnPos{ 0 };
	float angle = 0.0f;

	virtual Entity create(Scene& scene) override;
};

