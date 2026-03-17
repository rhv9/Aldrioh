#pragma once
#include <Core/Layer.h>
#include <Scene/Scene.h>

#include <UI/UIManager.h>
#include <Game/UI/UIStat.h>

struct StatModifier;

class PauseMenuLayer : public Layer
{
public:
	PauseMenuLayer(const std::string& name) : Layer(name) {}
	virtual ~PauseMenuLayer() override {}

	virtual void OnBegin() override;
	virtual void OnUpdate(Timestep delta) override;
	virtual void OnRender(Timestep delta) override;
	virtual void OnImGuiRender(Timestep delta) override;

	virtual void OnRemove() override {}

	virtual void OnTransitionIn() override;
	virtual void OnTransitionOut() override;

	virtual void OnKeyEvent(KeyEventArg& e) override;
	virtual void OnMouseButtonEvent(MouseButtonEventArg& e) override;
	virtual void OnMouseMoveEvent(MouseMoveEventArg& e) override;
	virtual void OnWindowResizeEvent(WindowResizeEventArg& e) override;

	bool IsPaused() const { return gamePaused; }

	void UpdateStats(const StatModifier& statModifier);

private:
	void ExitPauseMenuToGame();

	UIStat* uiStat = nullptr;

	UIStatIndex hp, dmg, critChance, critDmg, cooldown, luck;

	std::shared_ptr<Scene> scene;
	std::unique_ptr<UIManager> uiManager = nullptr;
	bool gamePaused = false;
};