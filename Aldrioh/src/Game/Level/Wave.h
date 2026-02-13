#pragma once
#include "IWaveLogic.h"
#include <Game/Entity/GameEntities.h>

class SpawnerWave : public IWaveLogic
{
public:
	SpawnerWave(Level& level, EnemyEntityType* enemyType) : IWaveLogic(level), type(enemyType) {}
	virtual ~SpawnerWave() override {}
	virtual void OnCreate() override {}
	virtual void OnUpdate(Timestep ts) override;
	virtual void OnFinish() override {}

	float timer = 0.0f;
	float cooldown = 0.2f;
	EnemyEntityType* type;
};