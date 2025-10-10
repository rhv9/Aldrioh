#pragma once
#include <Core/Layer.h>
#include <Scene/Scene.h>

#include <Events/EventHandler.h>
#include <UI/UIManager.h>

class Level;

class GameLayer : public Layer
{
public:
	GameLayer();

	virtual void OnBegin();
	virtual void OnUpdate(Timestep delta);
	virtual void OnImGuiRender(Timestep delta) override;

	virtual void OnRemove();

	void OnKeyPressed(KeyPressedEventArg& e);
	virtual void OnTransitionIn() override;
	virtual void OnTransitionOut() override;

private:
	std::shared_ptr<Scene> scene;

	EventCallbackID<KeyPressedEventArg> callbackKeyPressedID;
};