#include <pch.h>
#include "UIObject.h"

#include <Graphics/Renderer.h>
#include "UIManager.h"	

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
	child->RecalculateRenderPos();
}

UIManager* UIObject::GetUIManager()
{
	if (uiManager == nullptr && parent != nullptr)
		return uiManager = parent->GetUIManager();
	return nullptr;
}

void UIObject::OnMouseMove(MouseMoveEventArg& e)
{
	if (IsMouseWithin({ e.XPos, e.YPos }))
		LOG_CORE_INFO("Mouse hovering {}, pos:({},{})", name, e.XPos, e.YPos);

}

void UIObject::OnMouseMoveChildren(MouseMoveEventArg& e)
{
	for (UIObject* child : children)
	{
		if (child->IsEnabled())
		{
			child->OnMouseMove(e);
			child->OnMouseMoveChildren(e);
		}
	}
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

void UIObject::RecalculateRenderPos()
{
	glm::vec2 offset = parent != nullptr ? parent->GetRenderPos() : glm::vec2(0);
	glm::vec2 containerSize = parent != nullptr ? parent->size : (uiManager != nullptr ? uiManager->GetUIArea() : glm::vec2(1));

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

bool UIObject::IsMouseWithin(const glm::vec2& pos) const
{
	return pos.x >= renderPos.x && pos.x <= renderPos.x + size.x
		&& pos.y >= renderPos.y && pos.y <= renderPos.y + size.y;
}

