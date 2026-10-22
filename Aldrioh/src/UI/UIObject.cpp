#include <pch.h>
#include "UIObject.h"

#include <Graphics/Renderer.h>
#include "UIManager.h"
#include <imgui.h>
#include <Math/Math.h>

UIObject::UIObject(const std::string& name, const glm::vec2& relativePos, const glm::vec2& size)
	: name(name), relativePos(relativePos), size(size)
{
}

UIObject::~UIObject()
{
	for (UIObject* component : children)
		delete component;
}

void UIObject::OnUpdate(Timestep ts)
{
	for (UIObject* obj : children)
	{
		if (obj->IsEnabled())
			obj->OnUpdate(ts);
	}
}

void UIObject::OnRender(Timestep ts)
{
	if (backgroundColour != glm::vec4(0))
	{
		Renderer::UIDrawRectangle({ UIData::PIXEL, renderPos }, { UIData::PIXEL, size }, backgroundColour);
	}
}

void UIObject::AddChild(UIObject* child)
{
	child->SetParent(this);
	children.push_back(child);
	child->RecalculateInternalState();
}

void UIObject::SetScalingBasedWidth(const float percentage)
{
	ASSERT(percentage >= 0 && percentage <= 1.0f, "Scaling is out of bounds");
	scalingSize.x = percentage;
	RecalculateInternalState();
}

void UIObject::SetScalingBasedHeight(const float percentage)
{
	ASSERT(percentage >= 0 && percentage <= 1.0f, "Scaling is out of bounds");
	scalingSize.y = percentage;
	RecalculateInternalState();
}

UIObject* UIObject::GetFirstChild(UIType type)
{
	for (UIObject* uiObject : children)
		if (uiObject->GetType() == type)
			return uiObject;
	return nullptr;
}

UIManager* UIObject::GetUIManager()
{
	if (uiManager == nullptr && parent != nullptr)
		return uiManager = parent->GetUIManager();
	return uiManager;
}

void UIObject::LoadSectionData(SimpleStorage& ss, const Section& section)
{
	for (UIObject* obj : children)
	{
		std::string sectionName = std::format("{}-{}", section.name, obj->GetName());
		Section sectionChild = ss.GetSection(sectionName);
		obj->LoadSectionData(ss, sectionChild);
	}

	if (section.HasProperties())
	{
		for (auto data : section.datas)
		{
			if (glm::vec2* vecTwo = Section::GetPropertyIf<glm::vec2>("Position", data))
			{
				this->SetRelativePos(*vecTwo);
			}
			else if (glm::vec2* vecTwo = Section::GetPropertyIf<glm::vec2>("Size", data))
			{
				this->SetSize(*vecTwo);
			}
			else if (int* num = Section::GetPropertyIf<int>("AnchorPoint", data))
			{
				this->SetAnchorPoint(*num);
			}
		}
	}
}

void UIObject::SaveSectionData(SimpleStorage& ss, Section& section)
{
	for (UIObject* obj : children)
	{
		Section childSection;
		childSection.name = std::format("{}-{}", section.name, obj->GetName());
		obj->SaveSectionData(ss, childSection);
	}

	section.datas.emplace_back("Position", GetRelativePos());
	section.datas.emplace_back("Size", GetSize());
	section.datas.emplace_back("AnchorPoint", GetAnchorPoint().serialize());
	if (backgroundColour != DEFAULT_BACKGROUNDCOL)
		section.datas.emplace_back("BackgroundCol", backgroundColour);
	if (!enabled)
		section.datas.emplace_back("Enabled", false);

	ss.WriteSection(section);
}

void UIObject::ImGuiDraw()
{
}


void UIObject::RenderChildren(Timestep ts)
{
	for (UIObject* obj : children)
	{
		if (obj->IsEnabled())
		{
			obj->OnRender(ts);
			obj->RenderChildren(ts);
		}
	}
}

void UIObject::RecalculateInternalState()
{
	glm::vec2 offset = parent != nullptr ? parent->GetRenderPos() : glm::vec2(0);
	glm::vec2 containerSize = parent != nullptr ? parent->size : (uiManager != nullptr ? uiManager->GetUIArea() : glm::vec2(1));

	glm::vec2 anchorPos = anchorPoint.ConvertPos(relativePos, size, containerSize);
	renderPos = offset + anchorPos;

	if (scalingSize.x != std::numeric_limits<float>::min())
		size.x = scalingSize.x * containerSize.x;
	if (scalingSize.y != std::numeric_limits<float>::min())
		size.y = scalingSize.y * containerSize.y;

	for (UIObject* obj : children)
	{
		obj->RecalculateInternalState();
	}
}

void UIObject::SetUIManager(UIManager* uiManager)
{
	this->uiManager = uiManager;
	RecalculateInternalState();
}

bool UIObject::IsMouseHovering()
{
	return IsPosWithin(GetUIManager()->GetMousePos());
}

bool UIObject::IsPosWithin(const glm::vec2& pos) const
{
	return pos.x >= renderPos.x && pos.x <= renderPos.x + size.x
		&& pos.y >= renderPos.y && pos.y <= renderPos.y + size.y;
}

