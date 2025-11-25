#include "pch.h"
#include "EntityCameraController.h"

#include "Scene/Components.h"

#include <Game/Systems/RenderSystems.h>
#include <Game.h>
#include <Math/Math.h>


EntityCameraController::EntityCameraController(const float aspectRatio, const float zoomLevel)
	: CameraController(aspectRatio, zoomLevel)
{
}

void EntityCameraController::OnUpdate(Timestep delta)
{
	float frameDelta = Game::Instance().GetDelta();

	if (entity.IsValid())
	{
		glm::vec2 pos = EntitySystem::CalculateEntityTransformWithInterpolation(entity, delta);
		glm::vec2 diff = currentPosition - pos;
		float diffLength = glm::length(diff);
		
		glm::vec2 toMove = (currentPosition - pos) * Math::min(frameDelta * percentSpeed * 1.0f, 1.0f);

		if (glm::length(toMove) <= tolerance)
			currentPosition = pos;
		else
			currentPosition -= toMove;
		SetPosition(currentPosition);
	}
}

EntityCameraController::~EntityCameraController()
{
}

