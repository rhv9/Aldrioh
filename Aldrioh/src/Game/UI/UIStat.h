#pragma once
#include <UI/UIObject.h>

class SubTexture;
class UIImage;
class UIText;

enum class UIStatIndex : uint8_t
{
};

struct UIStatItem
{
	UIImage* image;
	UIText* name;
	UIText* val;
};

class UIStat : public UIObject
{
public:
	UIStat(const std::string& name, const glm::vec2& relativePos, const glm::vec2& size);

	virtual void OnUpdate(Timestep ts) override;
	virtual void OnRender(Timestep ts) override;
	virtual UIType GetType() const override { return UIType::Custom; }

	UIStatIndex AddItem(const std::string& name, SubTexture* subTexture, int base, int multiplier);

	void AddGap(float gap = 2.5f) { cumulateYOffset += gap; }
private:
	std::vector<UIStatItem> elements;
	float cumulateYOffset = 0.0f;
};