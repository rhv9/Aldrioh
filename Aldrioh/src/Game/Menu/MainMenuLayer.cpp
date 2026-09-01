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
#include <Audio/SoundManager.h>
#include <Game/GameSound.h>

#include <Game/Entity/GameEntities.h>

static CameraController backgroundCameraController(1920 / 1080.0f, 50.0f);
static glm::vec2 backgroundWindowSize{ 0 };
static uint32_t backgroundPixelHeight = 300;

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
	uiManager = std::make_unique<UIManager>();

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
	startButton->SetButtonColour(glm::vec4{ 0.1f, 0.1f, 0.1f, 1.0f });
	startButton->SetHoverColour(0.15f);
	startButton->SetOnClickCallback([this](UIButton* button) {
		LOG_INFO("MainMenu - switching to game layer");
		GlobalLayers::game = new GameLayer("Game Layer");
		this->QueueTransitionTo(GlobalLayers::game);
		});
	uiManager->AddUIObject(startButton);
	
	UIButton* exitButton = new UIButton("Exit Button", glm::vec2{ 0, -15 }, glm::vec2{ 20, 10 });
	exitButton->GetUIText()->SetText("Exit");
	exitButton->GetUIText()->SetFontSize(4);
	exitButton->GetUIText()->GetFontStyle().colour = Colour::WHITE;
	exitButton->SetAnchorPoint(AnchorPoint::CENTER);
	exitButton->SetButtonColour(glm::vec4{ 0.1f, 0.1f, 0.1f, 1.0f });
	exitButton->SetHoverColour(0.15f);
	exitButton->SetOnClickCallback([](UIButton* button) {
		LOG_CORE_INFO("MainMenu - Shutdown");
		Game::Instance().Shutdown();
		});
	uiManager->AddUIObject(exitButton);

	UIButton* upgradeMenuButton = new UIButton("Upgrade Button", glm::vec2{ 0, -25 }, glm::vec2{ 30, 10 });
	upgradeMenuButton->GetUIText()->SetText("Upgrade");
	upgradeMenuButton->GetUIText()->SetFontSize(4);
	upgradeMenuButton->GetUIText()->GetFontStyle().colour = Colour::WHITE;
	upgradeMenuButton->SetAnchorPoint(AnchorPoint::RIGHT_CENTER);
	upgradeMenuButton->SetButtonColour(glm::vec4{ 0.1f, 0.1f, 0.1f, 1.0f });
	upgradeMenuButton->SetHoverColour(0.15f);
	upgradeMenuButton->SetOnClickCallback([](UIButton* button) {
		LOG_INFO("MainMenu - switching to upgrade layer");
		GlobalLayers::upgradeMenu = new UpgradeMenuLayer("Upgrade Layer");
		GlobalLayers::mainMenu->QueueTransitionTo(GlobalLayers::upgradeMenu);
		});
	uiManager->AddUIObject(upgradeMenuButton);

	UpdateBackground(backgroundPixelHeight * (Game::Instance().GetWindow()->GetWidth() / (float)Game::Instance().GetWindow()->GetHeight()), backgroundPixelHeight);
}

void MainMenuLayer::OnUpdate(Timestep delta)
{
	scene->OnUpdate(delta);
	uiManager->OnUpdate(delta);
}

void MainMenuLayer::OnRender(Timestep delta)
{
	static float x = 0.0f;
	Renderer::SetClearColour(Colour::BLACK);
	scene->OnRender(delta);
	x += float(delta) * 0.01f;
	Renderer::DrawBackgroundPass({x, 0});
	Renderer::StartScene({ backgroundCameraController.GetCamera().GetViewProjection() });
	SubTexture subTexture = Renderer::GetBackgroundPassTexture()->GetAsSubTexture();
	Renderer::DrawQuad(glm::vec3{ 0, 0, 0.5f }, &subTexture, backgroundWindowSize);
	Renderer::EndScene();
	
	Renderer::StartUIScene();
	uiManager->OnRender(delta);
	Renderer::EndUIScene();
}

void MainMenuLayer::OnImGuiRender(Timestep delta)
{
}

void MainMenuLayer::OnRemove()
{
}

void MainMenuLayer::OnTransitionIn()
{
	delete GlobalLayers::game;
	GlobalLayers::game = nullptr;
	delete GlobalLayers::upgradeMenu;
	GlobalLayers::upgradeMenu = nullptr;
	uiManager->OnTransitionIn();
	LOG_CORE_INFO("Transitioned into main menu");
}

void MainMenuLayer::OnTransitionOut()
{
	LOG_CORE_INFO("Detaching!");
}

void MainMenuLayer::OnMouseButtonEvent(MouseButtonEventArg& e)
{
	uiManager->OnMouseButton(e);
}

void MainMenuLayer::OnMouseMoveEvent(MouseMoveEventArg& e)
{
	uiManager->OnMouseMove(e);
}

void MainMenuLayer::OnWindowResizeEvent(WindowResizeEventArg& e)
{
	UpdateBackground(backgroundPixelHeight * (e.Width / (float)e.Height), backgroundPixelHeight);

	uiManager->OnWindowResize(e);
}

void MainMenuLayer::OnKeyEvent(KeyEventArg& e)
{
	if (e.IsPressed(Input::KEY_ESCAPE))
	{
		LOG_CORE_INFO("Shutdown");
		Game::Instance().Shutdown();
	}
}

void MainMenuLayer::UpdateBackground(int width, int height)
{
	backgroundCameraController.OnResize(width, height);
	backgroundCameraController.SetZoomLevel(height / 2.0f);
	float x = backgroundCameraController.GetAspectRatio() * backgroundCameraController.GetZoomLevel();
	float y = backgroundCameraController.GetZoomLevel();
	backgroundCameraController.SetPosition({ x, y });
	backgroundWindowSize = { width, height };

	Renderer::ResizeBackgroundPass(width, height);
}
