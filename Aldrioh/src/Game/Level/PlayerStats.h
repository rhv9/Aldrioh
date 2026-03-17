#pragma once
#include <Events/EventHandler.h>

#include "Item/StatModifier.h"
class Level;

struct PlayerStatsEventArg : EventArg
{
	Level& level;
	PlayerStatsEventArg(Level& level) : level(level) {}
};

class PlayerStats
{
public:
	PlayerStats(Level& level) : level(level) {}

	void Init(float maxExp, float expMultiplier);
	void AddExp(float value);
	void AddExpMultiplier(float value) { expMultiplier += value; }

	float GetExpPercent() const { return exp / maxExp; }

	uint16_t GetLevelCount() const { return levelCount; }
	float GetCurrentExp() const { return exp; }
	float GetMaxExp() const { return maxExp; }
	StatModifier& GetStatModifier() { return statModifier; }

	EventHandler<PlayerStatsEventArg> expGainEventHandler;
	EventHandler<PlayerStatsEventArg> lvlUpEventHandler;
private:
	Level& level;
	float exp = 0;
	float maxExp = 100;
	float expMultiplier = 1.0f;
	float maxExpGrowth = 1.05f;
	uint16_t levelCount = 0;

	StatModifier statModifier;
};