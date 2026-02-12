#include <pch.h>
#include "LevelCounter.h"

void LevelCounter::incrementEnemyDeath(EntityID eType)
{
	if (enemyDeaths.find(eType.id) == enemyDeaths.end())
		enemyDeaths.insert({ eType.id, 0 });

	++enemyDeaths[eType.id];
}
