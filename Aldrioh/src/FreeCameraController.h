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
	virtual void OnMouseButtonCallback(MouseButtonEventArg& e) override;
	virtual void OnMouseMoveCallback(MouseMoveEventArg& e) override;
	virtual void OnMouseScrollCallback(MouseScrolledEventArg& e) override;
};
