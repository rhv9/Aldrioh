#include "pch.h"
#include "EntityCameraController.h"

#include "Scene/Components.h"

#include <Game/Systems/RenderSystems.h>


EntityCameraController::EntityCameraController(const float aspectRatio, const float zoomLevel)
	: CameraController(aspectRatio, zoomLevel)
{
}

void EntityCameraController::OnUpdate(Timestep delta)
{
	if (entity.IsValid())
	{
		glm::vec2 pos = EntitySystem::CalculateEntityTransformWithInterpolation(entity, delta);
		SetPosition({ pos.x, pos.y});
	}
}

EntityCameraController::~EntityCameraController()
{
}

