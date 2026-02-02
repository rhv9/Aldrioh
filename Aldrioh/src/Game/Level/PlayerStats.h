#pragma once

class Level;

class PlayerStats
{
public:
	PlayerStats(Level& level) : level(level) {}

	void Init(float maxExp, float expMultiplier);
	void AddExp(float value);
	void AddExpMultiplier(float value) { expMultiplier += value; }

	float GetExpPercent() const { return exp / maxExp; }
	uint16_t GetLevelCount() const { return levelCount; }

private:
	Level& level;
	float exp = 0;
	float maxExp = 100;
	float expMultiplier = 1.0f;
	float maxExpGrowth = 1.05f;
	uint16_t levelCount = 0;

};