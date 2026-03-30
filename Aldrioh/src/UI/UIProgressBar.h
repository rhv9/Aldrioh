#pragma once
#include "UIText.h"
class UIProgressBar : public UIObject
{
public:
	UIProgressBar(const std::string& name, const glm::vec2& relativePos, const glm::vec2& size);

	virtual void OnUpdate(Timestep ts) override;
	virtual void OnRender(Timestep ts) override;
	virtual UIType GetType() const override { return UIType::Button; }

	void SetProgress(float percentage);
	float GetProgress() const { return percentage; }

	void SetBarColour(const glm::vec4& colour) { barColour = colour; }
	void SetSmoothBar(float timeSeconds);

private:
	float currentTime = 0.0f;
	float durationSeconds = -1.0f;
	float maxTime = -1.0f;
	float prevPercentage = 0.0f;
	float percentage = 0.75f;
	glm::vec4 barColour = Colour::RED;
};