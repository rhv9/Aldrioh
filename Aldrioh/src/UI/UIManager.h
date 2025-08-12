#pragma once
#include "UIObject.h"

class UIManager
{
public:
	UIManager();
	~UIManager();

	void OnUpdate(Timestep ts);
	void OnRender();

	void AddUIObject(UIObject* object);
private:
	std::vector<UIObject*> uiObjects;
};