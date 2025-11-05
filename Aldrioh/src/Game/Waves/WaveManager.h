#pragma once

#include "Objective.h"

class Scene;
class Level;

class WaveManager
{
public:
	WaveManager(Scene& scene, Level& level);

	void Init();
	void OnUpdate(Timestep ts);
	
private:
	Scene& scene;
	Level& level;

	std::vector<Objective> objectives;
	int currentObjIndex = 0;
};