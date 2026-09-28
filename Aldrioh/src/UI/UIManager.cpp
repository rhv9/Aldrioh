#include <pch.h>
#include "UIManager.h"
#include <Core/Window.h>
#include <Graphics/Renderer.h>
#include <Game.h>
#include <Math/Math.h>

#include <Input/Input.h>

#include <imgui.h>

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

void UIManager::PollAndUpdateWindowSize()
{
	uiArea = Renderer::UIGetWindowSize();
	windowSizeCached = Game::Instance().GetWindow()->GetSize();

	for (UIObject* obj : uiObjects)
		obj->RecalculateInternalState();
}

void UIManager::OnImGuiRender(Timestep delta)
{
	if (!editorModeActive)
		return;
	static bool open = true;

	ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

	ImGui::Begin("UI Editing window", &open);

	selectedFound = false;
	int id = 0;
	for (int i = 0; i < uiObjects.size(); ++i)
	{
		UIObject* obj = uiObjects[i];
		ImGuiDrawTreeUIObject(obj, id);
	}

	if (!selectedFound)
		selectedObject = nullptr;

	ImGui::End();
	
	ImGui::Begin("UIObject editor", &open);

	if (selectedObject)
	{
		const int BUFFER_SIZE = 100;
		char bufferName[BUFFER_SIZE + 1];
		const std::string& name = selectedObject->GetName();
		for (int i = 0; i < Math::min(BUFFER_SIZE, name.size()); ++i)
			bufferName[i] = name[i];

		bufferName[Math::min(BUFFER_SIZE, name.size())] = '\0';

		ImGui::InputText("Name", bufferName, BUFFER_SIZE);
	}

	ImGui::End();
}

void UIManager::ImGuiDrawTreeUIObject(UIObject* obj, int& id)
{
	ImGui::PushID(++id);
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;

	if (obj == selectedObject)
	{
		selectedFound = true;
		flags |= ImGuiTreeNodeFlags_Selected;
	}

	if (obj->HasChildren())
		flags |= ImGuiTreeNodeFlags_OpenOnArrow;
	else
		flags |= ImGuiTreeNodeFlags_Leaf;

	bool open = ImGui::TreeNodeEx(obj->GetName().c_str(), flags);

	if (ImGui::IsItemClicked())
	{
		selectedObject = obj;
		selectedFound = true;
	}
	if (open)
	{
		for (UIObject* childObj : obj->children)
			ImGuiDrawTreeUIObject(childObj, id);

		ImGui::TreePop();
	}
	ImGui::PopID();
}



