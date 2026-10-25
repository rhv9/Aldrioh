#pragma once
#include "UIObject.h"

#include <Events/Events.h>

// TODO: UIManager needs Renderer::SetUIPixelHeight set first for it to set effect.

class UIManager
{
public:
	UIManager(const std::string_view newName);
	~UIManager();

	void OnUpdate(Timestep ts);
	void OnRender(Timestep ts);

	void AddUIObject(UIObject* object);

	const glm::vec2& GetUIArea() { return uiArea; }
	const glm::vec2 GetMousePos() const;

	void OnTransitionIn() { PollAndUpdateWindowSize(); }

	void OnWindowResize(WindowResizeEventArg& e);
	void OnMouseMove(MouseMoveEventArg& e);
	void OnMouseButton(MouseButtonEventArg& e);
	void OnKeyEvent(KeyEventArg& e);

	void OnImGuiRender(Timestep delta);
	void SetEditorModeActive(bool active);
	void ImGuiDrawTreeUIObject(UIObject* obj, int& id);

	void SaveToFile();
	void LoadFromFile();

	void SetEditorGridSize(float val) { editorMouseGridSize = val; }
	void DisableEditorGridSize() { editorMouseGridSize = -1.0f; }

private:
	void PollAndUpdateWindowSize();

	bool firstRender = true;

	std::string name;
	std::vector<UIObject*> uiObjects;
	glm::vec2 uiArea{ 0 }, windowSizeCached{ 0 };

	bool editorModeActive = false;
	glm::vec2 editorHeldPos{ 0.0f }, editorSelectedOriginalPos{ 0.0f };
	bool editorMouseHeld;
	UIObject* editorSelectedObject = nullptr;
	bool editorSelectedFound = false;
	float editorMouseGridSize = -1.0f;
};