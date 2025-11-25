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

	float maxDistance = 0.0f;
	float tolerance = 0.01f;
	float percentSpeed = 1.0f;
private:
	Entity entity;
};