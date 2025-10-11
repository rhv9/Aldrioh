#include <pch.h>
#include "UIManager.h"
#include <Core/Window.h>
#include <Graphics/Renderer.h>
#include <Game.h>

#include <Input/Input.h>

UIManager::UIManager()
{
	PollAndUpdateWindowSize();
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

const glm::vec2 UIManager::GetMousePos() const
{
	glm::vec2 mousePos = Input::GetMousePosition();
	float scaleX = uiArea.x / windowSizeCached.x;
	float scaleY = uiArea.y / windowSizeCached.y;

	return glm::vec2(mousePos.x * scaleX, mousePos.y * scaleY);
}



void UIManager::OnWindowResize(WindowResizeEventArg& e)
{
	PollAndUpdateWindowSize();
}

void UIManager::OnMouseMove(MouseMoveEventArg& e)
{
	float scaleX = uiArea.x / windowSizeCached.x;
	float scaleY = uiArea.y / windowSizeCached.y;

	MouseMoveEventArg relative{ e.XPos * scaleX, e.YPos * scaleY };

	for (UIObject* obj : uiObjects)
	{
		if (obj->IsEnabled())
		{
			obj->OnMouseMoveEvent(relative);
			obj->OnMouseMoveEventChildren(relative);
		}
	}
}

void UIManager::OnMouseButton(MouseButtonEventArg& e)
{

	for (UIObject* obj : uiObjects)
	{
		if (obj->IsEnabled())
		{
			obj->OnMouseButtonEvent(e);
			obj->OnMouseButtonEventChildren(e);
		}
	}
}

void UIManager::AttachEventListeners()
{
	callbackWindowResizeID = Game::Instance().GetWindow()->WindowResizeEventHandler += EVENT_BIND_MEMBER_FUNCTION(UIManager::OnWindowResize);
	callbackMouseMoveID = Game::Instance().GetWindow()->MouseMoveEventHandler += EVENT_BIND_MEMBER_FUNCTION(UIManager::OnMouseMove);
	callbackMouseButtonID = Game::Instance().GetWindow()->MouseButtonEventHandler += EVENT_BIND_MEMBER_FUNCTION(UIManager::OnMouseButton);

	PollAndUpdateWindowSize();
}

void UIManager::DetachEventListeners()
{
	callbackWindowResizeID.~EventCallbackID();
	callbackMouseMoveID.~EventCallbackID();
	callbackMouseButtonID.~EventCallbackID();
}

void UIManager::PollAndUpdateWindowSize()
{
	uiArea = Renderer::UIGetWindowSize();
	windowSizeCached = Game::Instance().GetWindow()->GetSize();

	for (UIObject* obj : uiObjects)
		obj->RecalculateRenderPos();
}


