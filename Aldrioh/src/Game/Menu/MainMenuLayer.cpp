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
	startButton->SetOnClickCallback([](UIButton* button) {

		});
	uiManager->AddUIObject(startButton);
	
	UIButton* exitButton = new UIButton("Exit Button", glm::vec2{ 0, -15 }, glm::vec2{ 20, 10 });
	exitButton->GetUIText()->SetText("Exit");
	exitButton->GetUIText()->SetFontSize(4);
	exitButton->GetUIText()->GetFontStyle().colour = Colour::WHITE;
	exitButton->SetAnchorPoint(AnchorPoint::CENTER);
	exitButton->SetBackgroundColour(glm::vec4{ 0.1f, 0.1f, 0.1f, 1.0f });
	exitButton->SetOnClickCallback([](UIButton* button) {
		Game::Instance().Shutdown();
		});
	uiManager->AddUIObject(exitButton);
}

void MainMenuLayer::OnUpdate(Timestep delta)
{
	scene->OnUpdate(delta);
	scene->OnRender(delta);
	
	Renderer::StartUIScene();

	uiManager->OnUpdate(delta);
	uiManager->OnRender(delta);

	Renderer::EndUIScene();
}

void MainMenuLayer::OnImGuiRender(Timestep delta)
{

}

void MainMenuLayer::OnRemove()
{
}
