#pragma once
#include "IWaveLogic.h"
#include <Game/Entity/GameEntities.h>

struct SpawnerWaveConfig
{
	float cooldownMin = 0.0f, cooldownMax = 1.0f;
	float maxEntities = 5;
};

class SpawnerWave : public IWaveLogic
{
public:
	SpawnerWave(Level& level, const SpawnerWaveConfig& config, EnemyEntityType* enemyType) : IWaveLogic(level), config(config), type(enemyType) {}
	virtual ~SpawnerWave() override {}
	virtual void OnCreate() override {}
	virtual void OnUpdate(Timestep ts) override;
	virtual void OnFinish() override {}

	float timer = 0.0f;
	const SpawnerWaveConfig config;
	EnemyEntityType* type;
};