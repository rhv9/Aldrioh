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

	int* world;
	int width, height;

	std::shared_ptr<CameraController> cameraController;
	std::shared_ptr<Scene> scene;
	entt::entity player;

	CollisionDispatcher collisionDispatcher;

	float shootTimer = 0.0f;
};