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

void MainMenuLayer::OnBegin()
{
	scene = std::make_shared<Scene>();

	// Camera
	float aspectRatio = static_cast<float>(Game::Instance().GetWindow()->GetHeight()) / Game::Instance().GetWindow()->GetWidth();
	std::unique_ptr<CameraController> cameraController = std::make_unique<CameraController>(aspectRatio, 1.0f);
	cameraController->SetZoomLevel(10);
	cameraController->SetPosition({ 0, 0 });
	// Add camera component
	Entity cameraEntity = scene->CreateEntity("RoamAndEntityCamera");
	cameraEntity.AddComponent<CameraComponent>(std::move(cameraController));
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
		this->QueueTransitionTo(GlobalLayers::game);
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
	Renderer::SetClearColour(Colour::BLACK);
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

void MainMenuLayer::OnKey(KeyEventArg& e)
{
	if (e.IsPressed(Input::KEY_ESCAPE))
	{
		LOG_CORE_INFO("Shutdown");
		Game::Instance().Shutdown();
	}
}

void MainMenuLayer::OnTransitionIn()
{
	callbackKeyID = Game::Instance().GetWindow()->KeyEventHandler += EVENT_BIND_MEMBER_FUNCTION(MainMenuLayer::OnKey);
	if (uiManager != nullptr)
		uiManager->AttachEventListeners();
}

void MainMenuLayer::OnTransitionOut()
{
	LOG_CORE_INFO("Detaching!");
	callbackKeyID.~EventCallbackID();
	uiManager->DetachEventListeners();
}
