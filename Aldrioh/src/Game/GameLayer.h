#pragma once
#include "Core/Layer.h"
#include "FreeCameraController.h"

#include "Scene/Scene.h"

class GameLayer : public Layer
{
public:
	GameLayer();

	virtual void OnBegin();
	virtual void OnUpdate(Timestep delta);
	virtual void OnImGuiRender(Timestep delta) override;

	virtual void OnRemove();

private:
	std::shared_ptr<CameraController> cameraController;
};