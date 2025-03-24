#pragma once

#include "CameraController.h"

class EntityCameraController : public CameraController
{
public:
	EntityCameraController(const float aspectRatio, const float zoomLevel = 1.0f);

	virtual ~EntityCameraController() override;

	void SetEntity(entt::registry* registry, entt::entity entity);

	virtual void OnUpdate(Timestep delta) override;

private:
	entt::entity entity = entt::null;
	entt::registry* registry = nullptr;
};