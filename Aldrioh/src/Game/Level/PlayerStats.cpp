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
	PlayerStatsEventArg e{ level };
	if (exp >= maxExp)
	{
		levelCount += 1;
		exp -= maxExp;
		maxExp *= maxExpGrowth;
		expMultiplier += 0.1f;

		lvlUpEventHandler.Invoke(e);
	}
	expGainEventHandler.Invoke(e);
}
