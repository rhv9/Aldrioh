#include <pch.h>
#include "Wave.h"
#include <Game/Level/Level.h>
#include <Game/Entity/GameEntities.h>
#include <Math/Math.h>

void SpawnerWave::OnUpdate(Timestep ts)
{
	timer -= ts;
	if (timer < 0.0f)
	{
		timer += Math::Random::linearFloat(config.cooldownMin, config.cooldownMax);
		LOG_CORE_INFO("Enemy count: {}", level.GetLevelStats().GetAliveEnemyCount());
		if (level.GetLevelStats().GetAliveEnemyCount() < config.maxEntities)
		{

			glm::vec2 spawnPos = level.GenerateRandomSpawnCoords();
			type->create(level, spawnPos, 1, nullptr);
		}
	}
}
