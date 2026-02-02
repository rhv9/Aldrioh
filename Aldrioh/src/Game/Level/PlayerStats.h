#pragma once

class Level;

class PlayerStats
{
public:
	PlayerStats(Level& level) : level(level) {}

	void Init(float maxExp, float expMultiplier);
	void AddExp(float value);
	void AddExpMultiplier(float value) { expMultiplier += value; }

private:
	Level& level;
	float exp = 0;
	float maxExp = 100;
	float expMultiplier = 1.0f;
	float maxExpGrowth = 1.05f;
	int levelCount = 0;

};