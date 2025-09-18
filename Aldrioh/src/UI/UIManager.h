#pragma once
#include "UIObject.h"

#include <Events/Events.h>

class UIManager
{
public:
	UIManager();
	~UIManager();

	void OnUpdate(Timestep ts);
	void OnRender(Timestep ts);

	void AddUIObject(UIObject* object);

	const glm::vec2& GetUIArea() { return uiArea; }
private:
	void OnWindowResize(WindowResizeEventArg& e);
	void OnMouseMove(MouseMoveEventArg& e);

	std::vector<UIObject*> uiObjects;
	glm::vec2 uiArea{ 0 }, windowSizeCached{ 0 };

	EventCallbackID<WindowResizeEventArg> callbackWindowResizeID;
	EventCallbackID<MouseMoveEventArg> callbackMouseMoveID;
};