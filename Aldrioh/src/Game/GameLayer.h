#pragma once
#include <Core/Layer.h>
#include <Scene/Scene.h>

class Level;

class GameLayer : public Layer
{
public:
	GameLayer();

	virtual void OnBegin();
	virtual void OnUpdate(Timestep delta);
	virtual void OnImGuiRender(Timestep delta) override;

	virtual void OnRemove();

public:
	static constexpr int ID = 0;

private:
	std::shared_ptr<Scene> scene;
};