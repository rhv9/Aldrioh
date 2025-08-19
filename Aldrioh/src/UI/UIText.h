#pragma once
#include "UIObject.h"
#include "Font.h"

class UIText : public UIObject
{
public:
	UIText(const std::string& name, const glm::vec2& relativePos, const glm::vec2& size);

	virtual void OnUpdate(Timestep ts) override;
	virtual void OnRender(Timestep ts) override;
	virtual UIType GetType() const override { return UIType::Text; }

	void SetText(const std::string& text);
	const std::string& GetText() const { return text; }

	const FontStyle& GetFontStyle() const { return style; }
	void SetFontStyle(const FontStyle& newStyle) { this->style = style; }

	void SetFontSize(float fontSize);

private:
	std::string text = "";
	FontStyle style;
};