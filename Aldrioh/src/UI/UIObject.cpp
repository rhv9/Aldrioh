#include <pch.h>
#include "UIObject.h"

#include <Graphics/Renderer.h>
#include "UIManager.h"	

UIObject::UIObject(const glm::vec2& relativePos, const glm::vec2& size)
	: relativePos(relativePos), size(size)
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

void UIObject::OnRender()
{
	if (backgroundColour != glm::vec4(0))
	{
		Renderer::UIDrawRectangle({ UIData::PIXEL, renderPos }, { UIData::PIXEL, size }, backgroundColour);
	}

	for (UIObject* obj : children)
	{
		if (obj->IsEnabled())
			obj->OnRender();
	}
}

void UIObject::AddChild(UIObject* component)
{
	component->SetParent(this);
	children.push_back(component);
}

void UIObject::RecalculateRenderPos()
{
	glm::vec2 offset = parent != nullptr ? parent->GetRenderPos() : glm::vec2(0);
	glm::vec2 containerSize = parent != nullptr ? parent->size : uiManager->GetUIArea();

	glm::vec2 anchorPos = anchorPoint.ConvertPos(relativePos, size, containerSize);
	renderPos = offset + anchorPos;

	for (UIObject* obj : children)
	{
		obj->RecalculateRenderPos();
	}
}

void UIObject::SetUIManager(UIManager* uiManager)
{
	this->uiManager = uiManager;
	RecalculateRenderPos();
}

