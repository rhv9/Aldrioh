#pragma once
#include "Game/GameLayer.h"
#include "FreeCameraController.h"

class Level
{
public:
	Level();
	~Level();

	void OnTick(Timestep delta);
	void OnRender(Timestep delta);

	void OnImGuiRender(Timestep delta);

private:

	int* world;
	int width, height;

	std::shared_ptr<CameraController> cameraController;
	entt::registry registry;
	entt::entity player;

	float shootTimer = 0.0f;
};