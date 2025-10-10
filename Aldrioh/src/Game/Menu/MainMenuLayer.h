#pragma once
#include <Core/Layer.h>
#include <Scene/Scene.h>

#include <UI/UIManager.h>

class MainMenuLayer : public Layer
{
public:
	MainMenuLayer();

	virtual void OnBegin() override;
	virtual void OnUpdate(Timestep delta) override;
	virtual void OnImGuiRender(Timestep delta) override;

	virtual void OnRemove() override;

	virtual void OnTransitionIn() override;
	virtual void OnTransitionOut() override;

	void OnKeyPressed(KeyPressedEventArg& e);

private:
	std::shared_ptr<Scene> scene;

	UIManager* uiManager;

	EventCallbackID<KeyPressedEventArg> callbackKeyPressedID;
};