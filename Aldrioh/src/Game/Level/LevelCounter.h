#pragma once
#include <Game/Entity/EntityType.h>

class LevelCounter
{
public:
	void incrementEnemyDeath(EntityID eType);


private:
	std::unordered_map<entitytypeid_t, uint32_t> enemyDeaths;
};