#include <pch.h>
#include "PauseMenuLayer.h"

#include <UI/Font.h>

#include <UI/UIManager.h>
#include <UI/UIObject.h>
#include <UI/UIText.h>
#include <UI/UIImage.h>
#include <UI/UIButton.h>

#include <Graphics/Renderer.h>
#include <Scene/CameraController.h>
#include <Scene/Components.h>

#include <Game.h>
#include <Scene/Entity.h>

#include <Game/GlobalLayers.h>


void PauseMenuLayer::OnBegin()
{
	Renderer::SetClearColour(Colour::BLACK);

	Renderer::SetUIPixelHeight(100);
	uiManager = new UIManager();

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
	continueButton->SetBackgroundColour(glm::vec4{ 0.1f, 0.1f, 0.1f, 1.0f });
	continueButton->SetOnClickCallback([this](UIButton* button) {
		this->ExitPauseMenuToGame();
		});
	uiManager->AddUIObject(continueButton);

	UIButton* exitButton = new UIButton("Exit Button", glm::vec2{ 0, -15 }, glm::vec2{ 35, 10 });
	exitButton->GetUIText()->SetText("Main Menu");
	exitButton->GetUIText()->SetFontSize(4);
	exitButton->GetUIText()->GetFontStyle().colour = Colour::WHITE;
	exitButton->SetAnchorPoint(AnchorPoint::CENTER);
	exitButton->SetBackgroundColour(glm::vec4{ 0.1f, 0.1f, 0.1f, 1.0f });
	exitButton->SetOnClickCallback([this](UIButton* button) {
		LOG_CORE_INFO("Pause menu - Switching to main menu");
		Game::Instance().GetLayerStack().QueuePopLayer(this);
		GlobalLayers::game->QueueTransitionTo(GlobalLayers::mainMenu);
		});
	uiManager->AddUIObject(exitButton);

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

void PauseMenuLayer::OnRemove()
{
	delete uiManager;
}

void PauseMenuLayer::OnKey(KeyEventArg& e)
{
	if (e.IsPressed(Input::KEY_ESCAPE))
	{
		ExitPauseMenuToGame();
	}
}

void PauseMenuLayer::ExitPauseMenuToGame()
{
	LOG_CORE_INFO("Pause menu - popping pause menu");
	Game::Instance().GetLayerStack().QueuePopLayer(this);
}

void PauseMenuLayer::OnTransitionIn()
{
	LOG_CORE_INFO("Pause menu - Transition In");
	callbackKeyID = Game::Instance().GetWindow()->KeyEventHandler += EVENT_BIND_MEMBER_FUNCTION(PauseMenuLayer::OnKey);
	uiManager->AttachEventListeners();
}

void PauseMenuLayer::OnTransitionOut()
{
	LOG_CORE_INFO("Pause menu - Transition Out");
	callbackKeyID.~EventCallbackID();
	uiManager->DetachEventListeners();
}
