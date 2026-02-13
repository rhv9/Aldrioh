#include <pch.h>
#include "LevelStats.h"

void LevelStats::onEnemyKilled(EntityID typeId)
{
	if (enemyKilled.find(typeId.id) == enemyKilled.end())
		enemyKilled.insert({ typeId.id, 0 });
	++enemyKilled[typeId.id];
	--aliveEnemy[typeId.id];

	--totalLiveEnemyCount;
	++totalEnemyKilled;
}

void LevelStats::addEntityCount(EntityID typeId)
{
	if (aliveEnemy.find(typeId.id) == aliveEnemy.end())
		aliveEnemy.insert({ typeId.id, 0 });
	++aliveEnemy[typeId.id];

	++totalLiveEnemyCount;
}

void LevelStats::onEnemyWipedByBorder(EntityID typeId)
{
	if (aliveEnemy.find(typeId.id) == aliveEnemy.end())
		return;

	--totalLiveEnemyCount;
	--aliveEnemy[typeId.id];
}
