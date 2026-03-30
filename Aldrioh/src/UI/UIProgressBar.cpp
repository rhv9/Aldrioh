#include "pch.h"
#include "UIProgressBar.h"
#include <Graphics/Renderer.h>

#include <Game.h>

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
	currentTime += Game::Instance().GetDelta();
	float renderPercent = percentage;
	if (currentTime <= maxTime)
	{
		float progress = 1.0f - ((maxTime - currentTime) / durationSeconds);
		float percentageDiff = percentage - prevPercentage;

		renderPercent = prevPercentage + progress * percentageDiff;
	}

	UIObject::OnRender(ts);
	Renderer::UIDrawRectangle({ UIData::PIXEL, renderPos }, { UIData::PIXEL, {size.x * renderPercent, size.y} }, barColour);
}

void UIProgressBar::SetProgress(float newPercentage)
{
	if (currentTime < maxTime)
	{
		float progress = 1.0f - ((maxTime - currentTime) / durationSeconds);
		float percentageDiff = percentage - prevPercentage;

		float renderPercent = prevPercentage + progress * percentageDiff;

		prevPercentage = renderPercent;
		percentage = newPercentage;
		maxTime = currentTime + durationSeconds;
	}
	else
	{
		if (durationSeconds > 0.0f)
			maxTime = currentTime + durationSeconds;
		prevPercentage = percentage;
		percentage = newPercentage;

	}
}

void UIProgressBar::SetSmoothBar(float timeSeconds)
{
	durationSeconds = timeSeconds;
}
