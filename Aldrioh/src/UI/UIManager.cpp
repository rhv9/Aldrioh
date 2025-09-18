#include <pch.h>
#include "UIManager.h"
#include <Core/Window.h>
#include <Graphics/Renderer.h>
#include <Game.h>

UIManager::UIManager()
{
	callbackWindowResizeID = Game::Instance().GetWindow()->WindowResizeEventHandler += EVENT_BIND_MEMBER_FUNCTION(UIManager::OnWindowResize);
	callbackMouseMoveID =  Game::Instance().GetWindow()->MouseMoveEventHandler += EVENT_BIND_MEMBER_FUNCTION(UIManager::OnMouseMove);

	uiArea = Renderer::UIGetWindowSize();
	windowSizeCached = Game::Instance().GetWindow()->GetSize();
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

void UIManager::OnRender(Timestep ts)
{
	for (UIObject* obj : uiObjects)
	{
		if (obj->IsEnabled())
		{
			obj->OnRender(ts);
			obj->RenderChildren(ts);
		}
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
	windowSizeCached = Game::Instance().GetWindow()->GetSize();

	for (UIObject* obj : uiObjects)
		obj->RecalculateRenderPos();
}

void UIManager::OnMouseMove(MouseMoveEventArg& e)
{
	float scaleX = uiArea.x / windowSizeCached.x;
	float scaleY = uiArea.y / windowSizeCached.y;

	MouseMoveEventArg relative{e.XPos * scaleX, e.YPos * scaleY};

	for (UIObject* obj : uiObjects)
	{
		if (obj->IsEnabled())
		{
			obj->OnMouseMove(relative);
			obj->OnMouseMoveChildren(relative);
		}
	}
}


