#include <pch.h>
#include "UpgradeMenuLayer.h"
#include <UI/UIText.h>
#include <Graphics/Renderer.h>

#include <Input/Input.h>
#include <Game/GlobalLayers.h>

void UpgradeMenuLayer::OnBegin()
{
	UIText* uiText = new UIText("Test", {0.0f, 0.0f}, {0, 0});
	uiText->SetAnchorPoint(AnchorPoint::CENTER);
	uiText->SetText("Why Hello There!");
	uiText->SetFontSize(8);
	uiText->GetFontStyle().colour = Colour::WHITE;

	uiManager.AddUIObject(uiText);
	uiManager.SetEditorModeActive(true);
}

void UpgradeMenuLayer::OnUpdate(Timestep delta)
{
	uiManager.OnUpdate(delta);
}

void UpgradeMenuLayer::OnRender(Timestep delta)
{
	Renderer::SetClearColour(Colour::BLACK);

	Renderer::StartUIScene();
	uiManager.OnRender(delta);
	Renderer::EndUIScene();
}

void UpgradeMenuLayer::OnImGuiRender(Timestep delta)
{
	uiManager.OnImGuiRender(delta);
}

void UpgradeMenuLayer::OnRemove()
{
}

void UpgradeMenuLayer::OnTransitionIn()
{
	uiManager.OnTransitionIn();
}

void UpgradeMenuLayer::OnTransitionOut()
{
}


void UpgradeMenuLayer::OnMouseButtonEvent(MouseButtonEventArg& e)
{
	uiManager.OnMouseButton(e);
}

void UpgradeMenuLayer::OnMouseMoveEvent(MouseMoveEventArg& e)
{
	uiManager.OnMouseMove(e);
}

void UpgradeMenuLayer::OnWindowResizeEvent(WindowResizeEventArg& e)
{
	uiManager.OnWindowResize(e);
}

void UpgradeMenuLayer::OnKeyEvent(KeyEventArg& e)
{
	if (e.IsPressed(Input::KEY_ESCAPE))
	{
		this->QueueTransitionTo(GlobalLayers::mainMenu);
	}
}

