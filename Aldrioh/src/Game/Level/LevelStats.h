#pragma once
#include <Game/Entity/EntityType.h>

class Level;

class LevelStats
{
public:
	void incrementEnemyDeath(EntityID eType);

private:
	std::unordered_map<entitytypeid_t, uint32_t> enemyDeaths;

	friend Level;
};