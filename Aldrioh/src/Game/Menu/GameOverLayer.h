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
	virtual void OnImGuiRender(Timestep delta) override {}

	virtual void OnRemove() override {}

	virtual void OnTransitionIn() override {}
	virtual void OnTransitionOut() override {}

	virtual void OnMouseButtonEvent(MouseButtonEventArg& e);
	virtual void OnMouseMoveEvent(MouseMoveEventArg& e);
	virtual void OnWindowResizeEvent(WindowResizeEventArg& e);
	virtual void OnKeyEvent(KeyEventArg& e);

private:
	std::unique_ptr<Scene> scene;
	float timeElapsed = 0.0f;
	bool displayTitle = true;
	bool currentDisplayTitleState = true;

	std::unique_ptr<UIManager> uiManager = nullptr;
	UIText* title = nullptr;
};