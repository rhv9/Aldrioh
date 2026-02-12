#pragma once

enum SpawnState
{
	REGULAR,
	BURST,
	FULL
};

class Wave
{
public:
	std::string name{ "" };
	float durationSeconds;
	int maxEntities;
	int burstUntil;
};
