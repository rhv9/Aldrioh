#include <pch.h>
#include "UIStat.h"

#include <UI/UIImage.h>
#include <UI/UIText.h>

UIStat::UIStat(const std::string& name, const glm::vec2& relativePos, const glm::vec2& size) : UIObject(name, relativePos, size)
{
	elements.reserve(10);
}

void UIStat::OnUpdate(Timestep ts)
{
	UIObject::OnUpdate(ts);
}

void UIStat::OnRender(Timestep ts)
{
	UIObject::OnRender(ts);
}

UIStatIndex UIStat::AddItem(const std::string& name, SubTexture* subTexture, int base, int multiplier)
{
	UIStatItem statItem;
	float xo = 1.0f;
	float yo = 1.0f;
	float imageSize = 5.0f;
	float gap = -2.25f;

	float yOffset = cumulateYOffset;

	// hp, dmg, crit chance, crit dmg, luck, cooldown
	statItem.image = new UIImage(std::format("{} image", name), { xo, yo + yOffset }, { imageSize, imageSize });
	statItem.image->SetSubTexture(subTexture);
	statItem.image->SetAnchorPoint(AnchorPoint::LEFT_TOP);
	AddChild(statItem.image);

	statItem.name = new UIText(std::format("{} name", name), { xo + imageSize + 0.25f, yo + yOffset + 1.5f }, glm::vec2(0));
	statItem.name->SetAnchorPoint(AnchorPoint::LEFT_TOP);
	statItem.name->GetFontStyle().WithColour(Colour::WHITE).WithSize(2.0f).WithCharSpacingPercent(0.77f);
	statItem.name->SetText(name);
	AddChild(statItem.name);

	statItem.val = new UIText(std::format("{} value", name), { xo + 0.25f, yo + yOffset + 1.5f }, glm::vec2(0));
	statItem.val->SetAnchorPoint(AnchorPoint::RIGHT_TOP);
	statItem.val->GetFontStyle().WithColour(Colour::WHITE).WithSize(2.0f).WithCharSpacingPercent(0.77f);
	statItem.val->SetText(std::format("+{} +{}%", base, multiplier));
	AddChild(statItem.val);

	elements.push_back(statItem);
	cumulateYOffset += imageSize + gap;


	SetSize({ size.x, yo + (cumulateYOffset + imageSize + gap) });

	return static_cast<UIStatIndex>(elements.size() - 1);
}

