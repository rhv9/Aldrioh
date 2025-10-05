#pragma once
#include <Scene/Scene.h>

class Level
{
public:

	Level(Scene& scene) : scene(scene) {};
	virtual ~Level() {};
	void virtual OnUpdate(Timestep ts) = 0;
	void virtual OnRender(Timestep ts) = 0;

	Scene& scene;

};