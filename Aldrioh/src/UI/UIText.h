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

	FontStyle& GetFontStyle() { return style; }
	void SetFontStyle(const FontStyle& newStyle) { this->style = style; SetText(text); }
	void SetFontSize(float fontSize);

	// text wrapping
	void SetTextWrap(bool enable) { shouldTextWrap = enable; SetText(text); }
	void SetTextWrapMaxWidth(float width) { textWrapMaxWidth = width; SetText(text); }

private:
	std::string text = "";
	FontStyle style;

	bool shouldTextWrap = false;
	float textWrapMaxWidth = 100.0f;

	// This is based on the number of lines, need to offset starting character position
	float cachedRenderYOffset = 0.0f;
};