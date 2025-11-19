#pragma once
#include <Core/Layer.h>
#include <Scene/Scene.h>

#include <Events/EventHandler.h>
#include <UI/UIManager.h>
#include <UI/UIText.h>

class Level;

class GameLayer : public Layer
{
public:
	GameLayer();

	virtual void OnBegin();
	virtual void OnUpdate(Timestep delta) override;
	virtual void OnRender(Timestep delta) override;
	virtual void OnImGuiRender(Timestep delta) override;

	virtual void OnRemove();

	void OnKey(KeyEventArg& e);
	void OnWindowResize(WindowResizeEventArg& e);
	void OnMouseButton(MouseButtonEventArg& e);
	virtual void OnTransitionIn() override;
	virtual void OnTransitionOut() override;

	UIText* GetUIScoreText() { return uiScoreText; }

private:
	std::shared_ptr<Scene> scene;

	UIManager* uiManager = nullptr;
	UIText* uiScoreText = nullptr;

	EventCallbackID<KeyEventArg> callbackKeyID;
	EventCallbackID<WindowResizeEventArg> windowResizeID;
	EventCallbackID<MouseButtonEventArg> callbackMouseButtonID;

};