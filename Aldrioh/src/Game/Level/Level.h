#pragma once
#include "Game/GameLayer.h"
#include "FreeCameraController.h"
#include <Components/CollisionDispatcher.h>
#include <Scene/Scene.h>

class Level
{
public:
	Level();
	~Level();

	void OnTick(Timestep delta);
	void OnRender(Timestep delta);
	void OnImGuiRender(Timestep delta);

private:
	std::shared_ptr<Scene> scene;

};