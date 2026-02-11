#pragma once
#include <Core/Layer.h>
#include <Scene/Scene.h>
#include <Events/EventHandler.h>
#include <Game/Level/Level.h>
#include "GameUILayer.h"

#include <UI/UIManager.h>
#include <UI/UIText.h>
#include <UI/UIProgressBar.h>



class GameLayer : public Layer
{
public:
	GameLayer(const std::string& name) : Layer(name) {}
	virtual ~GameLayer() override;

	virtual void OnBegin();
	virtual void OnUpdate(Timestep delta) override;
	virtual void OnRender(Timestep delta) override;
	virtual void OnImGuiRender(Timestep delta) override;
	virtual void OnRemove() {}

	virtual void OnTransitionIn() override;
	virtual void OnTransitionOut() override;
	virtual void OnPushedLayerAboveEvent() override;
	virtual void OnPoppedLayerIntoEvent() override;

	virtual void OnKeyEvent(KeyEventArg& e) override;
	virtual void OnMouseButtonEvent(MouseButtonEventArg& e) override;
	virtual void OnMouseMoveEvent(MouseMoveEventArg& e) override {}
	virtual void OnWindowResizeEvent(WindowResizeEventArg& e) override;

	void OnPlayerDeath();

	// Added it for efficiency sake. Why go through entt to find level entity when game can only have one level running.
	Level* GetCurrentLevel() { return currentLevel.get(); }
	GameUILayer* GetUILayer();

private:
	std::unique_ptr<Scene> scene = nullptr;
	std::unique_ptr<Level> currentLevel = nullptr;
};