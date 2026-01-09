#include "pch.h"
#include "UIProgressBar.h"
#include <Graphics/Renderer.h>

UIProgressBar::UIProgressBar(const std::string& name, const glm::vec2& relativePos, const glm::vec2& size) : UIObject(name, relativePos, size)
{
	backgroundColour = Colour::BLACK;
}

void UIProgressBar::OnUpdate(Timestep ts)
{
	UIObject::OnUpdate(ts);
}

void UIProgressBar::OnRender(Timestep ts)
{
	UIObject::OnRender(ts);
	Renderer::UIDrawRectangle({ UIData::PIXEL, renderPos }, { UIData::PIXEL, {size.x * percentage, size.y} }, barColour);
}
