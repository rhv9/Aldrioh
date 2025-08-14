#include <pch.h>
#include "UIManager.h"
#include <Core/Window.h>
#include <Graphics/Renderer.h>
#include <Game.h>

UIManager::UIManager()
{
	Game::Instance().GetWindow()->WindowResizeEventHandler += EVENT_BIND_MEMBER_FUNCTION(UIManager::OnWindowResize);

	uiArea = Renderer::UIGetWindowSize();
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
			obj->OnRender();
	}
}

void UIManager::AddUIObject(UIObject* object)
{
	object->SetUIManager(this);
	uiObjects.push_back(object);
}

void UIManager::OnWindowResize(WindowResizeEventArg& e)
{
	uiArea = Renderer::UIGetWindowSize();

	for (UIObject* obj : uiObjects)
		obj->RecalculateRenderPos();
}


