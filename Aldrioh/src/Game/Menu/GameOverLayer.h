#pragma once
#include <Core/Layer.h>
#include <Scene/Scene.h>

#include <UI/UIManager.h>
#include <UI/UIText.h>

class GameOverLayer : public Layer
{
public:
	GameOverLayer(const std::string& name) : Layer(name) {}

	virtual void OnBegin() override;
	virtual void OnUpdate(Timestep delta) override;
	virtual void OnRender(Timestep delta) override;
	virtual void OnImGuiRender(Timestep delta) override;

	virtual void OnRemove() override;

	virtual void OnTransitionIn() override;
	virtual void OnTransitionOut() override;

	void OnKey(KeyEventArg& e);

private:
	std::shared_ptr<Scene> scene;
	float timeElapsed = 0.0f;
	bool displayTitle = true;
	bool currentDisplayTitleState = true;

	UIManager* uiManager = nullptr;
	UIText* title = nullptr;

	EventCallbackID<KeyEventArg> callbackKeyID;
};