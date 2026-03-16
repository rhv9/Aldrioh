#pragma once
#include <Core/Layer.h>
#include <Scene/Scene.h>

#include <UI/UIManager.h>
#include <UI/UIText.h>
#include <UI/UIProgressBar.h>

#include <Game/Level/PlayerStats.h>

#include "Level/Item/Item.h"

class UIButton;
class UIImage;
class Item;

class GameUILayer : public Layer
{
public:
	GameUILayer(const std::string& name, Level& level) : Layer(name), level(level) {}

	virtual void OnBegin() override;
	virtual void OnUpdate(Timestep delta) override;
	virtual void OnRender(Timestep delta) override;
	virtual void OnImGuiRender(Timestep delta) override;
	virtual void OnRemove() override {}

	virtual void OnTransitionIn() override;
	virtual void OnTransitionOut() override {}

	virtual void OnMouseButtonEvent(MouseButtonEventArg& e) override;
	virtual void OnMouseMoveEvent(MouseMoveEventArg& e) override;
	virtual void OnWindowResizeEvent(WindowResizeEventArg& e) override;

	void OnLevelUpEvent(PlayerStatsEventArg& e);
	void OnExpGainEvent(PlayerStatsEventArg& e);

	void SelectLvlUpCard(int button);

	UIText* GetUILevelCountText() { return uiLevelCountText; }
	UIProgressBar* GetUIHealthProgressBar() { return uiPlayerHealthBar; }
	UIText* GetUITimerText() { return uiTimerText; }
	UIProgressBar* GetExpProgressBar() { return expProgressBar; }
	// Added it for efficiency sake. Why go through entt to find level entity when game can only have one level running.

	// Level up related
	void SetLvlUpUIActive(bool active);
	void SetLvlUpCardItem(int i, ItemID itemId, const LvlUpInfo& lvlUpInfo, const std::string& littleInfo);

private:
	Level& level;
	std::unique_ptr<UIManager> uiManager = nullptr;

	UIText* uiLevelCountText = nullptr;
	UIText* uiTimerText = nullptr;
	UIProgressBar* uiPlayerHealthBar = nullptr;
	UIProgressBar* expProgressBar = nullptr;

	// Lvl up related UI
	UIObject* lvlBackground = nullptr;
	std::array<UIButton*, 3> lvlupCards;
	std::array<UIImage*, 3> lvlupImages;
	std::array<UIText*, 3> lvlupTitles;
	std::array<UIText*, 3> lvlupLittleInfos;
	std::array<UIText*, 3> lvlupDescriptions;

	std::array<ItemID, 3> chosenItems;

	EventCallbackID<PlayerStatsEventArg> expGainCallbackId;
	EventCallbackID<PlayerStatsEventArg> lvlUpCallbackId;
};