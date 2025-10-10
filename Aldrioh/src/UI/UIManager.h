#pragma once
#include "UIObject.h"

#include <Events/Events.h>

// TODO: UIManager needs Renderer::SetUIPixelHeight set first for it to set effect.

class UIManager
{
public:
	UIManager();
	~UIManager();

	void OnUpdate(Timestep ts);
	void OnRender(Timestep ts);

	void AddUIObject(UIObject* object);

	const glm::vec2& GetUIArea() { return uiArea; }

	const glm::vec2 GetMousePos() const;

	void AttachEventListeners();
	void DetachEventListeners();
private:
	void OnWindowResize(WindowResizeEventArg& e);
	void OnMouseMove(MouseMoveEventArg& e);
	void OnMouseButton(MouseButtonEventArg& e);

	std::vector<UIObject*> uiObjects;
	glm::vec2 uiArea{ 0 }, windowSizeCached{ 0 };

	EventCallbackID<WindowResizeEventArg> callbackWindowResizeID;
	EventCallbackID<MouseMoveEventArg> callbackMouseMoveID;
	EventCallbackID<MouseButtonEventArg> callbackMouseButtonID;
};