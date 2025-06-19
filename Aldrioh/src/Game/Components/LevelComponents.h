#pragma once
#include <Game/Level/TestLevel.h>

struct LevelComponent
{
	Level* level;

	LevelComponent() = default;
	LevelComponent(const LevelComponent&) = default;
	LevelComponent(Level* level) : level(level) {}
};