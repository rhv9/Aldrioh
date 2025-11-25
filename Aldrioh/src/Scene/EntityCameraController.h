#pragma once

#include "CameraController.h"

#include <Scene/Entity.h>

class EntityCameraController : public CameraController
{
public:
	EntityCameraController(const float aspectRatio, const float zoomLevel = 1.0f);
	virtual ~EntityCameraController() override;

	void SetEntity(Entity e) { entity = e; }
	virtual void OnUpdate(Timestep delta) override;

	float maxDistance = 1.0f;
	float tolerance = 0.0001f;
	float percentSpeed = 0.9999f;

	glm::vec2 currentPosition{ 0 };
private:
	Entity entity;
};