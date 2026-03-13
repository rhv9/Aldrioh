#include <pch.h>
#include "UIText.h"

#include <Graphics/Renderer.h>
#include <Math/Math.h>

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
	if (!shouldTextWrap)
		Renderer::UIDrawText(style, text, { UIData::PIXEL, renderPos });
	else
		Renderer::UIDrawTextWithWrapping(style, text, { UIData::PIXEL, renderPos }, textWrapMaxWidth);

}

void UIText::SetText(const std::string& text)
{
	this->text = text;
	if (!shouldTextWrap)
		SetSize({ style.CalculateTextWidth(text), style.size });
	else
	{
		float fullTextWidth = style.CalculateTextWidth(text);
		float width = Math::min(textWrapMaxWidth, fullTextWidth);
		// This is roughly correct, can produce wrong height as not accounting for the initial full font size on each line, good enough for now
		int lineCount = Math::fceil(fullTextWidth / textWrapMaxWidth);
		float height = style.size + style.size * style.textWrappingLineSpacingPercent * (lineCount - 1);

		SetSize({ width, height });
		LOG_INFO("{}: UIText Size {}, line count: {}", name, glm::to_string(size), lineCount);
	}

}

void UIText::SetFontSize(float fontSize)
{
	style.size = fontSize;
	SetText(text);
}
