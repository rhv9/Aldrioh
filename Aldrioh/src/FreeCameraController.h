#pragma once

#include "Events/Events.h"
#include "Graphics/Camera.h"

#include "Scene/CameraController.h"


class FreeCameraController : public CameraController
{
public:
	FreeCameraController(const float aspectRatio, const float zoomLevel = 1.0f);

	virtual void OnUpdate(Timestep delta) override;

private:

	void OnMousePressedCallback(MouseButtonPressedEventArg& e);
	void OnMouseReleasedCallback(MouseButtonReleasedEventArg& e);
	void OnMouseMoveCallback(MouseMoveEventArg& e);
	void OnMouseScrollCallback(MouseScrolledEventArg& e);
	void OnWindowResizeCallback(WindowResizeEventArg& e);
};
