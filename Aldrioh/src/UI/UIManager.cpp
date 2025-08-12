#include <pch.h>
#include "UIManager.h"

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
	for (UIObject* obj : uiObjects)
		delete obj;
}

void UIManager::OnUpdate(Timestep ts)
{
	for (UIObject* obj : uiObjects)
	{
		if (obj->IsEnabled())
			obj->OnUpdate(ts);
	}
}

void UIManager::OnRender()
{
	for (UIObject* obj : uiObjects)
	{
		if (obj->IsEnabled())
			obj->OnRender(glm::vec2{0, 0});
	}
}

void UIManager::AddUIObject(UIObject* object)
{
	uiObjects.push_back(object);
}


