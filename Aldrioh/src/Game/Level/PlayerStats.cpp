#include <pch.h>
#include "PlayerStats.h"

#include "Level.h"

void PlayerStats::Init(float maxExp, float expMultiplier)
{
	this->maxExp = maxExp;
	this->expMultiplier = expMultiplier;
}

void PlayerStats::AddExp(float value)
{
	exp += value * expMultiplier;
	if (exp >= maxExp)
	{
		levelCount += 1;
		exp -= maxExp;
		maxExp *= maxExpGrowth;
		expMultiplier += 0.1f;

		level.OnLevelUp();
	}

	LOG_INFO("Level: {} Exp: {}", levelCount, exp);
}
