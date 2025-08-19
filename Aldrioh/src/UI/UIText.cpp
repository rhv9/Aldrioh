#include <pch.h>
#include "UIText.h"

#include <Graphics/Renderer.h>

UIText::UIText(const std::string& name, const glm::vec2& relativePos, const glm::vec2& size)
	: UIObject(name, relativePos, size)
{
}

void UIText::OnUpdate(Timestep ts)
{
	UIObject::OnUpdate(ts);
}

void UIText::OnRender(Timestep ts)
{
	UIObject::OnRender(ts);

	Renderer::UIDrawText(style.font, text, { UIData::PIXEL, renderPos }, style.size, style.colour, style.charSpacingPercent);

}

void UIText::SetText(const std::string& text)
{
	this->text = text;
	SetSize({ style.CalculateTextWidth(text), style.size });
}

void UIText::SetFontSize(float fontSize)
{
	style.size = fontSize;
	SetSize({ style.CalculateTextWidth(text), style.size });
}
