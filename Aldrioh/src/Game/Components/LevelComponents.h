#pragma once
#include <Game/Level/Level.h>

struct LevelComponent
{
	Level* level;

	LevelComponent() = default;
	LevelComponent(const LevelComponent&) = default;
	LevelComponent(Level* level) : level(level) {}
};

struct AddScoreComponent
{
	float points = 0;

	AddScoreComponent() = default;
	AddScoreComponent(float points) : points(points) {}
	AddScoreComponent(const AddScoreComponent&) = default;
};

struct ScoreComponent
{
	float score = 0.0f;
	bool updated = false;
	std::function<void(int newScore)> onScoreUpdate = nullptr;

	ScoreComponent() = default;
	ScoreComponent(std::function<void(float newScore)> onScoreUpdate) : onScoreUpdate(onScoreUpdate) {}
	ScoreComponent(const ScoreComponent&) = default;
};