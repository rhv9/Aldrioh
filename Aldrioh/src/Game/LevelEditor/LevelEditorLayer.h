#pragma once

#include <Core/Layer.h>
#include <Scene/Scene.h>

class LevelEditorLayer : public Layer
{
public:
	LevelEditorLayer();

	virtual void OnBegin();

	virtual void OnUpdate(Timestep delta) override;
	virtual void OnRender(Timestep delta) override;
	virtual void OnImGuiRender(Timestep delta);

	virtual void OnRemove();

private:
	std::shared_ptr<Scene> scene;
};