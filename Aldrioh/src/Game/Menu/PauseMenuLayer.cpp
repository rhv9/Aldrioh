#include <pch.h>
#include "PauseMenuLayer.h"

#include <UI/Font.h>

#include <UI/UIManager.h>
#include <UI/UIObject.h>
#include <UI/UIText.h>
#include <UI/UIImage.h>
#include <UI/UIButton.h>
#include <Game/UI/UIStat.h>

#include <Graphics/Renderer.h>
#include <Scene/CameraController.h>
#include <Scene/Components.h>

#include <Game.h>
#include <Scene/Entity.h>

#include <Game/GlobalLayers.h>

#include <Game/SpriteCollection.h>
#include <Game/Level/Item/StatModifier.h>


void PauseMenuLayer::OnBegin()
{
	Renderer::SetClearColour(Colour::BLACK);

	Renderer::SetUIPixelHeight(100);
	uiManager = std::make_unique<UIManager>();

	UIObject* uiBackground = new UIObject("Background", { 0,0 }, Renderer::UIGetWindowSize());
	uiBackground->SetBackgroundColour(glm::vec4{ 0.3f, 0.3f, 0.3f, 0.5f });
	uiBackground->SetScalingBasedWidth(1.0f);
	uiManager->AddUIObject(uiBackground);

	UIText* title = new UIText("Title", glm::vec2{ 0, 25 }, glm::vec2{ 3 });
	title->SetAnchorPoint(AnchorPoint::CENTER);
	title->SetText("Paused");
	title->SetFontSize(8);
	title->GetFontStyle().colour = Colour::WHITE;
	uiManager->AddUIObject(title);

	UIButton* continueButton = new UIButton("Continue Button", glm::vec2{ 0, 0 }, glm::vec2{ 30, 10 });
	continueButton->GetUIText()->SetText("Continue");
	continueButton->GetUIText()->SetFontSize(4);
	continueButton->GetUIText()->GetFontStyle().colour = Colour::WHITE;
	continueButton->SetAnchorPoint(AnchorPoint::CENTER);
	continueButton->SetButtonColour(glm::vec4{ 0.1f, 0.1f, 0.1f, 1.0f });
	continueButton->SetHoverColour(0.15f);
	continueButton->SetOnClickCallback([this](UIButton* button) {
		this->ExitPauseMenuToGame();
		});
	uiManager->AddUIObject(continueButton);

	UIButton* exitButton = new UIButton("Exit Button", glm::vec2{ 0, -15 }, glm::vec2{ 35, 10 });
	exitButton->GetUIText()->SetText("Main Menu");
	exitButton->GetUIText()->SetFontSize(4);
	exitButton->GetUIText()->GetFontStyle().colour = Colour::WHITE;
	exitButton->SetAnchorPoint(AnchorPoint::CENTER);
	exitButton->SetButtonColour(glm::vec4{ 0.1f, 0.1f, 0.1f, 1.0f });
	exitButton->SetHoverColour(0.15f);
	exitButton->SetOnClickCallback([this](UIButton* button) {
		LOG_CORE_INFO("Pause menu - Switching to main menu");
		Game::Instance().GetLayerStack().QueuePopLayer(this);
		GlobalLayers::game->QueueTransitionTo(GlobalLayers::mainMenu);
		});
	uiManager->AddUIObject(exitButton);


	// StatUI

	uiStat = new UIStat("Stat Main", { 5.0f, 15.0f }, { 40.0f, 70.0f });
	uiStat->SetBackgroundColour(Colour::RGBCol(22, 22, 22));
	uiStat->SetAnchorPoint(AnchorPoint::LEFT_TOP);
	uiManager->AddUIObject(uiStat);

	hp = uiStat->AddItem("Health", Sprites::get(Sprites::coin), 69, 69);
	dmg = uiStat->AddItem("Damage", Sprites::get(Sprites::coin), 69, 69);
	critChance = uiStat->AddItem("Crit luck", Sprites::get(Sprites::coin), 69, 69);
	critDmg = uiStat->AddItem("Crit Dmg", Sprites::get(Sprites::coin), 69, 69);
	uiStat->AddGap();
	luck = uiStat->AddItem("Luck", Sprites::get(Sprites::coin), 69, 69);
	cooldown = uiStat->AddItem("Cooldown", Sprites::get(Sprites::coin), 69, 69);
}

void PauseMenuLayer::OnUpdate(Timestep delta)
{
	uiManager->OnUpdate(delta);
}

void PauseMenuLayer::OnRender(Timestep delta)
{
	Renderer::StartUIScene();
	uiManager->OnRender(delta);
	Renderer::EndUIScene();
}

void PauseMenuLayer::OnImGuiRender(Timestep delta)
{
}

void PauseMenuLayer::UpdateStats(const StatModifier& statModifier)
{
	uiStat->SetItem(hp, statModifier.hp_base, statModifier.hp_multiplier);
	uiStat->SetItem(dmg, statModifier.dmg_base, statModifier.dmg_multiplier);
	uiStat->SetItem(critChance, statModifier.critChance_base, statModifier.critChance_multiplier);
	uiStat->SetItem(critDmg, statModifier.critDmg_base, statModifier.critDmg_multiplier);
	uiStat->SetItem(cooldown, statModifier.cooldown_base, statModifier.cooldown_multiplier);
	uiStat->SetItem(luck, statModifier.luck_modifier);
}

void PauseMenuLayer::ExitPauseMenuToGame()
{
	LOG_CORE_INFO("Pause menu - popping pause menu");
	Game::Instance().GetLayerStack().QueuePopLayer(this);
}

void PauseMenuLayer::OnTransitionIn()
{
	LOG_CORE_INFO("Pause menu - Transition In");
	if (GlobalLayers::game != nullptr)
		GlobalLayers::game->SetShouldUpdate(false);
	uiManager->OnTransitionIn();
	gamePaused = true;
}

void PauseMenuLayer::OnTransitionOut()
{
	LOG_CORE_INFO("Pause menu - Transition Out");
	if (GlobalLayers::game != nullptr)
		GlobalLayers::game->SetShouldUpdate(true);

	gamePaused = false;
}

void PauseMenuLayer::OnKeyEvent(KeyEventArg& e)
{
	if (e.IsPressed(Input::KEY_ESCAPE))
	{
		ExitPauseMenuToGame();
	}
	e.isHandled = true;
}

void PauseMenuLayer::OnMouseButtonEvent(MouseButtonEventArg& e)
{
	uiManager->OnMouseButton(e);
}

void PauseMenuLayer::OnMouseMoveEvent(MouseMoveEventArg& e)
{
	uiManager->OnMouseMove(e);
}

void PauseMenuLayer::OnWindowResizeEvent(WindowResizeEventArg& e)
{
	uiManager->OnWindowResize(e);
}
