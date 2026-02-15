#include "pch.h"
#include "GameUILayer.h"

#include <Graphics/Renderer.h>
#include <Game/GlobalLayers.h>

#include <Scene/Components.h>
#include <Game/Components/EntityComponents.h>
#include <UI/UIButton.h>

#include <Game/Systems/RenderSystems.h>
#include <Game.h>

void GameUILayer::OnBegin()
{
	expGainCallbackId = level.GetPlayerStats().expGainEventHandler += EVENT_BIND_MEMBER_FUNCTION(GameUILayer::OnExpGainEvent);
	lvlUpCallbackId = level.GetPlayerStats().lvlUpEventHandler += EVENT_BIND_MEMBER_FUNCTION(GameUILayer::OnLevelUpEvent);

	uiManager = std::make_unique<UIManager>();
	// Active UI for game
	{
		uiLevelCountText = new UIText("Level Count", { 2, 4 }, glm::vec2{ 0 });
		uiLevelCountText->SetText("Level: 0");
		uiLevelCountText->SetAnchorPoint(AnchorPoint::LEFT_TOP);
		uiLevelCountText->GetFontStyle().colour = Colour::WHITE;
		uiLevelCountText->SetFontSize(4);
		uiManager->AddUIObject(uiLevelCountText);

		uiPlayerHealthBar = new UIProgressBar("Player Health", { 2, 2 }, { 7, 0.4f });
		uiPlayerHealthBar->SetAnchorPoint(AnchorPoint::CENTER);
		uiPlayerHealthBar->SetBackgroundColour(Colour::GREY);
		uiPlayerHealthBar->SetProgress(1);
		uiManager->AddUIObject(uiPlayerHealthBar);

		expProgressBar = new UIProgressBar("Exp bar", { 0, 0 }, { 50, 2 });
		expProgressBar->SetAnchorPoint(AnchorPoint::LEFT_TOP);
		expProgressBar->SetBackgroundColour(Colour::GREY);
		expProgressBar->SetBarColour(Colour::GREEN);
		expProgressBar->SetScalingBasedWidth(1.0f);
		expProgressBar->SetProgress(0);
		uiManager->AddUIObject(expProgressBar);

		uiTimerText = new UIText("Timer", { 0, 4 }, glm::vec2{ 4, 4 });
		uiTimerText->SetText("");
		uiTimerText->SetAnchorPoint(AnchorPoint::CENTER_TOP);
		uiTimerText->GetFontStyle().colour = Colour::WHITE;
		uiTimerText->GetFontStyle().charSpacingPercent = 0.9f;
		uiTimerText->SetFontSize(4);
		uiManager->AddUIObject(uiTimerText);
	}

	// UI for lvl up system
	{
		lvlBackground = new UIObject("LVL Background", { 0.0f, 0.0f }, { 45.0f, 60.0f });
		lvlBackground->SetAnchorPoint(AnchorPoint::CENTER);
		lvlBackground->SetBackgroundColour(Colour::BLUE);
		uiManager->AddUIObject(lvlBackground);

		UIText* lvlUpTitle = new UIText("LVL up title", { 0.0f, 4.0f }, { 0.0f, 0.0f });
		lvlUpTitle->SetAnchorPoint(AnchorPoint::CENTER_TOP);
		lvlUpTitle->GetFontStyle().WithSize(4.0f).WithColour(Colour::WHITE).WithCharSpacingPercent(0.85f);
		lvlUpTitle->SetText("<!Level Up!>");
		lvlBackground->AddChild(lvlUpTitle);

		UIText* lvlUpExplanation = new UIText("LVL up explanation", { 0.0f, lvlUpTitle->GetRelativePosY() + lvlUpTitle->GetHeight() + 3.0f }, { 0.0f, 0.0f });
		lvlUpExplanation->SetAnchorPoint(AnchorPoint::CENTER_TOP);
		lvlUpExplanation->GetFontStyle().WithSize(2.0f).WithColour(Colour::WHITE).WithCharSpacingPercent(0.85f);
		lvlUpExplanation->SetText("Choose your upgrade!");
		lvlBackground->AddChild(lvlUpExplanation);

		float lvlUpHeightOffset = lvlUpTitle->GetRelativePosY() + lvlUpTitle->GetHeight();
		float yButtonHeight = 12.0f;
		float yOffset = 10.0f;
		float yGapBetween = 1.0f;
		for (int i = 0; i < 3; ++i)
		{
			float yRelativePos = lvlUpHeightOffset + yButtonHeight * i + yOffset + yGapBetween * i;
			UIButton* button = new UIButton(std::format("Option {}", i+1), { 0.0f, yRelativePos }, { 40.0f, yButtonHeight });
			button->SetAnchorPoint(AnchorPoint::CENTER_TOP);
			button->SetBackgroundColour(Colour::BLACK);
			lvlBackground->AddChild(button);

		}

	}
}

void GameUILayer::OnUpdate(Timestep delta)
{
	if (GlobalLayers::game == nullptr)
		return;

	// Update health bar
	Level* level = GlobalLayers::game->GetCurrentLevel();
	auto& hc = level->GetPlayer().GetComponent<HealthComponent>();
	uiPlayerHealthBar->SetProgress(hc.health / hc.maxHealth);

	// Update timer bar
	float elapsedTime = level->GetElapsedTime();
	int mins = static_cast<int>(elapsedTime / 60.0f);
	int seconds = static_cast<int>(elapsedTime) % 60;
	std::string timerText = std::format("{}:{:02}", mins, seconds);
	uiTimerText->SetText(timerText);

	uiManager->OnUpdate(delta);
}

void GameUILayer::OnRender(Timestep delta)
{
	// Update player health bar position
	UIProgressBar* uiPlayerHealthBar = GlobalLayers::game->GetUILayer()->GetUIHealthProgressBar();
	auto& playerCameraController = GlobalLayers::game->GetCurrentLevel()->GetPlayerCamera().GetComponent<CameraComponent>().cameraController;
	glm::vec2 playerPos = EntitySystem::CalculateEntityTransformWithInterpolation(GlobalLayers::game->GetCurrentLevel()->GetPlayer(), delta);
	glm::vec2 playerCameraPos = playerCameraController->GetPosition();
	glm::vec2 barPos = playerPos - playerCameraPos;
	barPos.y -= 0.7f;

	// TODO this does not need to be calculated every frame
	glm::vec2 uiArea = uiPlayerHealthBar->GetUIManager()->GetUIArea();
	glm::vec2 cameraArea = playerCameraController->GetBounds().GetSize();
	glm::vec2 cameraToUIMapping{ uiArea.x / cameraArea.x, uiArea.y / cameraArea.y };

	barPos.x *= cameraToUIMapping.x;
	barPos.y *= cameraToUIMapping.y;

	uiPlayerHealthBar->SetRelativePos(barPos);

	Renderer::StartUIScene();
	uiManager->OnRender(delta);
	Renderer::EndUIScene();
}

void GameUILayer::OnLevelUpEvent(PlayerStatsEventArg& e)
{
	uiLevelCountText->SetText(std::format("Level: {}", e.level.GetPlayerStats().GetLevelCount()));
}

void GameUILayer::OnExpGainEvent(PlayerStatsEventArg& e)
{
	expProgressBar->SetProgress(e.level.GetPlayerStats().GetExpPercent());
}

void GameUILayer::SetLvlUpUIActive(bool active)
{
	if (active)
		lvlBackground->Enable();
	else
		lvlBackground->Disable();
}


void GameUILayer::OnTransitionIn()
{
	uiManager->OnTransitionIn();
}

void GameUILayer::OnMouseButtonEvent(MouseButtonEventArg& e)
{
	uiManager->OnMouseButton(e);
	e.isHandled = GlobalLayers::pauseMenu->IsPaused();
}

void GameUILayer::OnMouseMoveEvent(MouseMoveEventArg& e)
{
	uiManager->OnMouseMove(e);
	e.isHandled = GlobalLayers::pauseMenu->IsPaused();
}

void GameUILayer::OnWindowResizeEvent(WindowResizeEventArg& e)
{
	uiManager->OnWindowResize(e);
}
