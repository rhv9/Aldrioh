#include "pch.h"
#include "TestSystems.h"
#include "HeadersRenderSystems.h"
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

	Renderer::DrawQuad({ 0, 0, 0.5f }, Sprites::get(Sprites::animBossUp[((int)Platform::GetElapsedTime()) % 4]));
	Renderer::DrawQuad({ 1, 0, 0.5f }, Sprites::get(Sprites::animBossDown[((int)Platform::GetElapsedTime()) % 4]));
	Renderer::DrawQuad({ 2, 0, 0.5f }, Sprites::get(Sprites::animBossLeft[((int)Platform::GetElapsedTime()) % 4]));
	Renderer::DrawQuad({ 3, 0, 0.5f }, Sprites::get(Sprites::animBossRight[((int)Platform::GetElapsedTime()) % 4]));
}
