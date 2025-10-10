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
private:
	UIText* buttonText = nullptr;
	UIButtonCallback callback = nullptr;

	bool pressedWithin = false;

};