#pragma once

struct LevelComponent
{
	Level* level;

	LevelComponent() = default;
	LevelComponent(const LevelComponent&) = default;
};