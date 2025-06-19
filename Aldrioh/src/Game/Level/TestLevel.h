#pragma once
#include "Level.h"
#include <Scene/Scene.h>

class TestLevel : public Level
{
public:

	TestLevel(Scene& scene);
	virtual ~TestLevel();
	void virtual OnUpdate(Timestep ts) override;
	void virtual OnRender(Timestep ts) override;

private:
	Scene& scene;

	int* tiles;
	int width, height;
};