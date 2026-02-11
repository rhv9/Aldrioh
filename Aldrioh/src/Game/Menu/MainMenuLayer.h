#pragma once
#include <Core/Layer.h>
#include <Scene/Scene.h>

#include <UI/UIManager.h>

class MainMenuLayer : public Layer
{
public:
	MainMenuLayer(const std::string& name) : Layer(name) {}
	virtual ~MainMenuLayer() override {}

	virtual void OnBegin() override;
	virtual void OnUpdate(Timestep delta) override;
	virtual void OnRender(Timestep delta) override;
	virtual void OnImGuiRender(Timestep delta) override;

	virtual void OnRemove() override;

	virtual void OnTransitionIn() override;
	virtual void OnTransitionOut() override;

	virtual void OnMouseButtonEvent(MouseButtonEventArg& e);
	virtual void OnMouseMoveEvent(MouseMoveEventArg& e);
	virtual void OnWindowResizeEvent(WindowResizeEventArg& e);
	virtual void OnKeyEvent(KeyEventArg& e);

private:
	std::shared_ptr<Scene> scene;

	UIManager* uiManager = nullptr;
};