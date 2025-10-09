#pragma once
#include <Core/Layer.h>
#include <Scene/Scene.h>

class MainMenuLayer : Layer
{
public:
	MainMenuLayer();

	virtual void OnBegin() override;
	virtual void OnUpdate(Timestep delta) override;
	virtual void OnImGuiRender(Timestep delta) override;

	virtual void OnRemove() override;

private:
	std::shared_ptr<Scene> scene;

};