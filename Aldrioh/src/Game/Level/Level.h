#pragma once

class Level
{
public:

	Level();

	void virtual OnUpdate(Timestep ts) = 0;
	void virtual OnRender(Timestep ts) = 0;

private:
	int* world;
	int width, height;
};