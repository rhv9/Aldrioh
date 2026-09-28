#include <pch.h>
#include "UpgradeMenuLayer.h"
#include <UI/UIText.h>
#include <UI/UIButton.h>
#include <Graphics/Renderer.h>

#include <Input/Input.h>
#include <Game/GlobalLayers.h>

void UpgradeMenuLayer::OnBegin()
{
	UIText* uiTitle = new UIText("Title", {0.0f, 0.0f}, {0, 0});
	uiTitle->SetAnchorPoint(AnchorPoint::CENTER);
	uiTitle->SetText("Laboratory");
	uiTitle->SetFontSize(8);
	uiTitle->GetFontStyle().colour = Colour::WHITE;
	uiManager.AddUIObject(uiTitle);

	UIText* uiName = new UIText("Name", { 0.0f, -3.0f }, { 0, 0 });
	uiName->SetAnchorPoint(AnchorPoint::CENTER);
	uiName->SetText("Schnitzen!");
	uiName->SetFontSize(2);
	uiName->GetFontStyle().colour = Colour::RED;
	uiManager.AddUIObject(uiName);

	UIButton* uiBackButton = new UIButton("Back_Button", { 1.0f, 1.0f }, {12.0f, 4.0f});
	uiBackButton->SetAnchorPoint(AnchorPoint::RIGHT_BOTTOM);
	uiBackButton->SetButtonColour(Colour::BLUE);
	uiBackButton->GetUIText()->GetFontStyle().WithColour(Colour::WHITE).WithSize(3);
	uiBackButton->GetUIText()->SetText("Back");
	uiBackButton->SetOnClickCallback([](UIButton* button) {
		LOG_INFO("Going back now!");
		});
	uiManager.AddUIObject(uiBackButton);

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

