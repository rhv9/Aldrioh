#pragma once
#include "UIObject.h"

struct WindowResizeEventArg;

class UIManager
{
public:
	UIManager();
	~UIManager();

	void OnUpdate(Timestep ts);
	void OnRender();

	void AddUIObject(UIObject* object);

	const glm::vec2& GetUIArea() { return uiArea; }
private:
	void OnWindowResize(WindowResizeEventArg& e);

	std::vector<UIObject*> uiObjects;
	glm::vec2 uiArea{ 0 };
};