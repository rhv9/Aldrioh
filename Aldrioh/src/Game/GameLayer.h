#pragma once
#include <Core/Layer.h>
#include <Scene/Scene.h>

#include <Events/EventHandler.h>
#include <UI/UIManager.h>
#include <UI/UIText.h>
#include <UI/UIProgressBar.h>

#include <Game/Level/Level.h>

class GameLayer : public Layer
{
public:
	GameLayer(const std::string& name) : Layer(name) {}

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
	virtual void OnPushedLayerAboveEvent() override;
	virtual void OnPoppedLayerIntoEvent() override;

	UIText* GetUIScoreText() { return uiScoreText; }
	UIProgressBar* GetUIHealthProgressBar() { return uiPlayerHealthBar; }
	UIText* GetUITimerText() { return uiTimerText; }
	// Added it for efficiency sake. Why go through entt to find level entity when game can only have one level running.
	Level* GetCurrentLevel() { return currentLevel.get(); }

private:
	std::unique_ptr<Scene> scene = nullptr;
	std::unique_ptr<Level> currentLevel = nullptr;

	UIManager* uiManager = nullptr;
	UIText* uiScoreText = nullptr;
	UIProgressBar* uiPlayerHealthBar = nullptr;
	UIText* uiTimerText = nullptr;

	EventCallbackID<KeyEventArg> callbackKeyID;
	EventCallbackID<WindowResizeEventArg> windowResizeID;
	EventCallbackID<MouseButtonEventArg> callbackMouseButtonID;

};