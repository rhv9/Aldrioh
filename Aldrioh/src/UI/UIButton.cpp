#include "pch.h"
#include "UIButton.h"

UIButton::UIButton(const std::string& name, const glm::vec2& relativePos, const glm::vec2& size)
	: UIObject(name, relativePos, size)
{
	buttonText = new UIText(name + "_text", { 0, 0 }, size);
	buttonText->SetAnchorPoint(AnchorPoint::CENTER);
	buttonText->SetText("");
	AddChild(buttonText);
}

void UIButton::OnUpdate(Timestep ts)
{
	UIObject::OnUpdate(ts);
}

void UIButton::OnRender(Timestep ts)
{
	UIObject::OnRender(ts);

}

void UIButton::OnMouseButtonEvent(MouseButtonEventArg& e)
{
	bool mouseHovering = IsMouseHovering();
	if (e.IsPressed(Input::MOUSE_BUTTON_1) && mouseHovering)
		pressedWithin = true;

	if (e.IsReleased(Input::MOUSE_BUTTON_1))
	{
		if (pressedWithin && mouseHovering)
		{
			if (callback != nullptr)
				callback(this);
		}
		pressedWithin = false;
	}

}
