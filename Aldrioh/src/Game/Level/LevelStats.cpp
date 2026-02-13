#include <pch.h>
#include "LevelStats.h"

void LevelStats::incrementEnemyDeath(EntityID eType)
{
	if (enemyDeaths.find(eType.id) == enemyDeaths.end())
		enemyDeaths.insert({ eType.id, 0 });

	++enemyDeaths[eType.id];
}
