#include "pch.h"
#include "TestSystems.h"
#include <Systems/HeadersRenderSystems.h>
#include <Input/Input.h>
#include <Core/Platform.h>

void EntitySystem::TestUpdateSystem(Timestep ts, Scene& scene)
{
}

void EntitySystem::TestRenderSystem(Timestep ts, Scene& scene)
{
	auto& cameraController = scene.GetPrimaryCameraEntity().GetComponent<CameraComponent>().cameraController;

	glm::vec2 mousePos = Input::GetMousePosition();
	glm::vec2 cameraPos = cameraController->GetPosition();
}
