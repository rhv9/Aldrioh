#include <pch.h>
#include "GameOverLayer.h"

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
#include <Math/Math.h>

#include <Game/GlobalLayers.h>


void GameOverLayer::OnBegin()
{
	scene = std::make_unique<Scene>();

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
	uiManager = std::make_unique<UIManager>();

	title = new UIText("Game Over", glm::vec2{ 0, 25 }, glm::vec2{ 3 });
	title->SetAnchorPoint(AnchorPoint::CENTER);
	title->SetFontSize(8);
	title->GetFontStyle().colour = Colour::WHITE;
	title->GetFontStyle().charSpacingPercent += 0.13f;
	title->SetText("</Game Over/>");
	uiManager->AddUIObject(title);

	UIButton* mainMenuButton = new UIButton("Main Menu Button", glm::vec2{ 0, -10.0f }, glm::vec2{ 35, 10 });
	mainMenuButton->GetUIText()->SetText("Main Menu");
	mainMenuButton->GetUIText()->SetFontSize(4);
	mainMenuButton->GetUIText()->GetFontStyle().colour = Colour::WHITE;
	mainMenuButton->SetAnchorPoint(AnchorPoint::CENTER);
	mainMenuButton->SetBackgroundColour(glm::vec4{ 0.1f, 0.1f, 0.1f, 1.0f });
	mainMenuButton->SetOnClickCallback([this](UIButton* button) {
		LOG_INFO("MainMenu - switching to main menu layer");
		this->QueueTransitionTo(GlobalLayers::mainMenu);
		});

	uiManager->AddUIObject(mainMenuButton);
}

void GameOverLayer::OnUpdate(Timestep delta)
{
	timeElapsed += delta;

	if (Math::floatMod(delta, 1.0f) < 0.5f)
		displayTitle = true;
	else
		displayTitle = false;

	if (currentDisplayTitleState != displayTitle)
	{
		currentDisplayTitleState = displayTitle;
		title->GetFontStyle().colour.w = currentDisplayTitleState ? 1.0f : 0.0f;
	}

	scene->OnUpdate(delta);
	uiManager->OnUpdate(delta);
}

void GameOverLayer::OnRender(Timestep delta)
{
	Renderer::SetClearColour(glm::vec4{ 0.04f, 0.0f, 0.0f, 1.0f });
	scene->OnRender(delta);

	Renderer::StartUIScene();
	uiManager->OnRender(delta);
	Renderer::EndUIScene();
}


void GameOverLayer::OnMouseButtonEvent(MouseButtonEventArg& e)
{
	uiManager->OnMouseButton(e);
}

void GameOverLayer::OnMouseMoveEvent(MouseMoveEventArg& e)
{
	uiManager->OnMouseMove(e);
}

void GameOverLayer::OnWindowResizeEvent(WindowResizeEventArg& e)
{
	uiManager->OnWindowResize(e);
}

void GameOverLayer::OnKeyEvent(KeyEventArg& e)
{
	if (e.IsPressed(Input::KEY_ESCAPE))
	{
		LOG_INFO("MainMenu - switching to main menu layer");
		QueueTransitionTo(GlobalLayers::mainMenu);
	}
}
