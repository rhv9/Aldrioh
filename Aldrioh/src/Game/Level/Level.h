#pragma once

class Level
{
public:
	Level();
	~Level();

	void OnUpdate(Timestep delta);

private:

	int* world;
	int width, height;

};