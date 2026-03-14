#pragma once
#include "UIText.h"


class UIButton : public UIObject
{
	using UIButtonCallback = std::function<void(UIButton* button)>;

public:
	UIButton(const std::string& name, const glm::vec2& relativePos, const glm::vec2& size);

	virtual void OnUpdate(Timestep ts) override;
	virtual void OnRender(Timestep ts) override;
	virtual UIType GetType() const override { return UIType::Button; }

	void SetOnClickCallback(UIButtonCallback callback) { this->callback = callback; }

	UIText* GetUIText() { return buttonText; }

	virtual void OnMouseButtonEvent(MouseButtonEventArg& e) override;
	virtual void OnMouseMoveEvent(MouseMoveEventArg& e) override;

	void SetButtonColour(const glm::vec4& colour);
	void SetHoverColour(float percent);

private:
	void UpdateButtonColour();
		
private:
	constexpr static float INACTIVE_HOVER_PERCENT_VALUE = std::numeric_limits<float>::min();

	UIText* buttonText = nullptr;
	UIButtonCallback callback = nullptr;

	bool pressedWithin = false;
	glm::vec4 normalColour = Colour::BLACK, hoverColour = Colour::WHITE;
	float hoverPercent = INACTIVE_HOVER_PERCENT_VALUE;
};