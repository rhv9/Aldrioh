#include <pch.h>
#include "MenuBackgroundLayer.h"
#include <UI/UIText.h>
#include <Graphics/Renderer.h>

#include <Input/Input.h>
#include <Game/GlobalLayers.h>
#include <Scene/EntityCameraController.h>

#include <Game.h>
#include <Scene/Components.h>

static CameraController backgroundCameraController(1920 / 1080.0f, 50.0f);
static glm::vec2 backgroundWindowSize{ 0 };
static uint32_t backgroundPixelHeight = 300;

void MenuBackgroundLayer::OnBegin()
{
	UpdateBackground(backgroundPixelHeight * (Game::Instance().GetWindow()->GetWidth() / (float)Game::Instance().GetWindow()->GetHeight()), backgroundPixelHeight);
}

void MenuBackgroundLayer::OnUpdate(Timestep delta)
{
	xOffsetCamera += 0.01f;
}

void MenuBackgroundLayer::OnRender(Timestep delta)
{
	Renderer::SetClearColour(Colour::BLACK);

	Renderer::DrawBackgroundPass({ xOffsetCamera, 0 });
	Renderer::StartScene({ backgroundCameraController.GetCamera().GetViewProjection() });
	SubTexture subTexture = Renderer::GetBackgroundPassTexture()->GetAsSubTexture();
	Renderer::DrawQuad(glm::vec3{ 0, 0, 0.5f }, &subTexture, backgroundWindowSize);
	Renderer::EndScene();
}

void MenuBackgroundLayer::OnImGuiRender(Timestep delta)
{
}

void MenuBackgroundLayer::OnRemove()
{
}

void MenuBackgroundLayer::OnTransitionIn()
{
}

void MenuBackgroundLayer::OnTransitionOut()
{
}


void MenuBackgroundLayer::OnMouseButtonEvent(MouseButtonEventArg& e)
{
}

void MenuBackgroundLayer::OnMouseMoveEvent(MouseMoveEventArg& e)
{
}

void MenuBackgroundLayer::OnWindowResizeEvent(WindowResizeEventArg& e)
{
	UpdateBackground(backgroundPixelHeight * (e.Width / (float)e.Height), backgroundPixelHeight);
}

void MenuBackgroundLayer::OnKeyEvent(KeyEventArg& e)
{
}

void MenuBackgroundLayer::UpdateBackground(int width, int height)
{
	backgroundCameraController.OnResize(width, height);
	backgroundCameraController.SetZoomLevel(height / 2.0f);
	float x = backgroundCameraController.GetAspectRatio() * backgroundCameraController.GetZoomLevel();
	float y = backgroundCameraController.GetZoomLevel();
	backgroundCameraController.SetPosition({ x, y });
	backgroundWindowSize = { width, height };

	Renderer::ResizeBackgroundPass(width, height);
}


