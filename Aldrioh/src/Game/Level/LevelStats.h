#pragma once
#include <Game/Entity/EntityType.h>

class Level;

class LevelStats
{
public:
	void onEnemyKilled(EntityID typeId);
	void addEntityCount(EntityID typeId);
	void onEnemyWipedByBorder(EntityID typeId);

private:
	std::unordered_map<entitytypeid_t, uint32_t> enemyKilled;
	std::unordered_map<entitytypeid_t, uint32_t> aliveEnemy;
	int totalLiveEnemyCount = 0;
	int totalEnemyKilled = 0;

	friend Level;
};