#include <pch.h>
#include "UIObject.h"

#include <Graphics/Renderer.h>

UIObject::UIObject(const glm::vec2& pos, const glm::vec2& size)
	: pos(pos), size(size)
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

void UIObject::OnRender(const glm::vec2& offset)
{
	glm::vec2 absolutePos = offset + pos;
	if (backgroundColour != glm::vec4(0))
		Renderer::UIDrawRectangle({ UIData::PIXEL, absolutePos }, { UIData::PIXEL, size }, backgroundColour);

	for (UIObject* obj : children)
	{
		if (obj->IsEnabled())
			obj->OnRender(absolutePos);
	}
}

void UIObject::AddChild(UIObject* component)
{
	component->SetParent(this);
	children.push_back(component);
}

