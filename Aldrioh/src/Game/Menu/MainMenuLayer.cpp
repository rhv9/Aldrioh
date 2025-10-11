#include <pch.h>
#include "MainMenuLayer.h"

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

MainMenuLayer::MainMenuLayer()
{
}

void MainMenuLayer::OnBegin()
{
	scene = std::make_shared<Scene>();
	Renderer::SetClearColour(Colour::BLACK);

	// Camera
	float aspectRatio = static_cast<float>(Game::Instance().GetWindow()->GetHeight()) / Game::Instance().GetWindow()->GetWidth();
	CameraController* cameraController = new CameraController(aspectRatio, 1.0f);
	cameraController->SetZoomLevel(10);
	cameraController->SetPosition({ 0, 0 });
	// Add camera component
	Entity cameraEntity = scene->CreateEntity("RoamAndEntityCamera");
	cameraEntity.AddComponent<CameraComponent>(cameraController);
	cameraEntity.RemoveComponent<TransformComponent>(); // TODO: Need to consider this pls

	Renderer::SetUIPixelHeight(100);
	uiManager = new UIManager();
	uiManager->AttachEventListeners();

	UIText* title = new UIText("Title", glm::vec2{ 0, 25 }, glm::vec2{ 3 });
	title->SetAnchorPoint(AnchorPoint::CENTER);
	title->SetText("<Aldrioh>");
	title->SetFontSize(8);
	title->GetFontStyle().colour = Colour::WHITE;
	uiManager->AddUIObject(title);

	UIButton* startButton = new UIButton("Start Button", glm::vec2{ 0, 0 }, glm::vec2{ 20, 10 });
	startButton->GetUIText()->SetText("Start");
	startButton->GetUIText()->SetFontSize(4);
	startButton->GetUIText()->GetFontStyle().colour = Colour::WHITE;
	startButton->SetAnchorPoint(AnchorPoint::CENTER);
	startButton->SetBackgroundColour(glm::vec4{ 0.1f, 0.1f, 0.1f, 1.0f });
	startButton->SetOnClickCallback([this](UIButton* button) {
		LOG_INFO("MainMenu - switching to game layer");
		this->TransitionTo(GlobalLayers::game);
		});
	uiManager->AddUIObject(startButton);
	
	UIButton* exitButton = new UIButton("Exit Button", glm::vec2{ 0, -15 }, glm::vec2{ 20, 10 });
	exitButton->GetUIText()->SetText("Exit");
	exitButton->GetUIText()->SetFontSize(4);
	exitButton->GetUIText()->GetFontStyle().colour = Colour::WHITE;
	exitButton->SetAnchorPoint(AnchorPoint::CENTER);
	exitButton->SetBackgroundColour(glm::vec4{ 0.1f, 0.1f, 0.1f, 1.0f });
	exitButton->SetOnClickCallback([](UIButton* button) {
		LOG_CORE_INFO("MainMenu - Shutdown");
		Game::Instance().Shutdown();
		});
	uiManager->AddUIObject(exitButton);
}

void MainMenuLayer::OnUpdate(Timestep delta)
{
	scene->OnUpdate(delta);
	uiManager->OnUpdate(delta);
}

void MainMenuLayer::OnRender(Timestep delta)
{
	scene->OnRender(delta);
	
	Renderer::StartUIScene();
	uiManager->OnRender(delta);
	Renderer::EndUIScene();
}

void MainMenuLayer::OnImGuiRender(Timestep delta)
{

}

void MainMenuLayer::OnRemove()
{
	delete uiManager;
}

void MainMenuLayer::OnKeyPressed(KeyPressedEventArg& e)
{
	if (e.Key == Input::KEY_ESCAPE)
	{
		LOG_CORE_INFO("Shutdown");
		Game::Instance().Shutdown();
	}
}

void MainMenuLayer::OnTransitionIn()
{
	callbackKeyPressedID = Game::Instance().GetWindow()->KeyPressedEventHandler += EVENT_BIND_MEMBER_FUNCTION(MainMenuLayer::OnKeyPressed);
	if (uiManager != nullptr)
		uiManager->AttachEventListeners();
}

void MainMenuLayer::OnTransitionOut()
{
	LOG_CORE_INFO("Detaching!");
	callbackKeyPressedID.~EventCallbackID();
	uiManager->DetachEventListeners();
}
