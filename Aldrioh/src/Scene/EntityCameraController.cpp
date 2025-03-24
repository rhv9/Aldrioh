#include "pch.h"
#include "EntityCameraController.h"

#include "Scene/Components.h"


EntityCameraController::EntityCameraController(const float aspectRatio, const float zoomLevel)
	: CameraController(aspectRatio, zoomLevel)
{
}

void EntityCameraController::OnUpdate(Timestep delta)
{
	TransformComponent& playerTransform = registry->get<TransformComponent>(entity);

	SetPosition({playerTransform.position.x, playerTransform.position.y});
	//LOG_TRACE("Camera player position {}", glm::to_string(tc.position));
}

EntityCameraController::~EntityCameraController()
{
}

void EntityCameraController::SetEntity(entt::registry* registry, entt::entity entity)
{
	this->entity = entity;
	this->registry = registry;
}

