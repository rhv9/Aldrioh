#include <pch.h>
#include "Wave.h"
#include <Game/Level/Level.h>
#include <Game/Entity/GameEntities.h>

void SpawnerWave::OnUpdate(Timestep ts)
{
	timer += ts;
	if (timer > cooldown)
	{
		timer -= cooldown;
		glm::vec2 spawnPos = level.GenerateRandomSpawnCoords();
		type->create(level, spawnPos, 1);
	}
}
