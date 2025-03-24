#pragma once

#include "Events/Events.h"
#include "Graphics/Camera.h"

#include "Scene/CameraController.h"


class FreeRoamEntityCameraController : public CameraController
{
public:
	FreeRoamEntityCameraController(const float aspectRatio, const float zoomLevel = 1.0f);

	void SetEntity(entt::registry* registry, entt::entity entity);

	virtual void OnUpdate(Timestep delta) override;

private:

	void OnMousePressedCallback(MouseButtonPressedEventArg& e);
	void OnMouseReleasedCallback(MouseButtonReleasedEventArg& e);
	void OnMouseMoveCallback(MouseMoveEventArg& e);
	void OnMouseScrollCallback(MouseScrolledEventArg& e);
	void OnWindowResizeCallback(WindowResizeEventArg& e);

	entt::entity entity = entt::null;
	entt::registry* registry = nullptr;
};
