#include <pch.h>
#include "MenuBackgroundLayer.h"
#include <UI/UIText.h>
#include <Graphics/Renderer.h>

#include <Input/Input.h>
#include <Game/GlobalLayers.h>

void MenuBackgroundLayer::OnBegin()
{
	UIText* uiText = new UIText("Test", { 0.0f, 0.0f }, { 0, 0 });
	uiText->SetAnchorPoint(AnchorPoint::CENTER);
	uiText->SetText("Why Hello There!");
	uiText->SetFontSize(8);
	uiText->GetFontStyle().colour = Colour::WHITE;

	uiManager.AddUIObject(uiText);
}

void MenuBackgroundLayer::OnUpdate(Timestep delta)
{
	uiManager.OnUpdate(delta);
}

void MenuBackgroundLayer::OnRender(Timestep delta)
{
	Renderer::SetClearColour(Colour::BLACK);

	Renderer::StartUIScene();
	uiManager.OnRender(delta);
	Renderer::EndUIScene();
}

void MenuBackgroundLayer::OnImGuiRender(Timestep delta)
{
}

void MenuBackgroundLayer::OnRemove()
{
}

void MenuBackgroundLayer::OnTransitionIn()
{
	uiManager.OnTransitionIn();
}

void MenuBackgroundLayer::OnTransitionOut()
{
}


void MenuBackgroundLayer::OnMouseButtonEvent(MouseButtonEventArg& e)
{
	uiManager.OnMouseButton(e);
}

void MenuBackgroundLayer::OnMouseMoveEvent(MouseMoveEventArg& e)
{
	uiManager.OnMouseMove(e);
}

void MenuBackgroundLayer::OnWindowResizeEvent(WindowResizeEventArg& e)
{
	uiManager.OnWindowResize(e);
}

void MenuBackgroundLayer::OnKeyEvent(KeyEventArg& e)
{
	if (e.IsPressed(Input::KEY_ESCAPE))
	{
		this->QueueTransitionTo(GlobalLayers::mainMenu);
	}
}

