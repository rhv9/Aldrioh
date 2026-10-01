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

	if (editorModeActive && selectedObject)
	{
		glm::vec2 offset = selectedObject->GetParent() ? selectedObject->GetParent()->GetRenderPos() : glm::vec2(0);
		glm::vec2 containerSize = selectedObject->GetParent() ? selectedObject->GetParent()->size : this->GetUIArea() ;

		glm::vec2 anchorPos = selectedObject->GetAnchorPoint().ConvertPos(glm::vec2(0), glm::vec2(1.0f), containerSize);
		glm::vec2 renderPos = offset + anchorPos;
		Renderer::UIDrawRectangle({ UIData::PIXEL, renderPos }, { UIData::PIXEL, glm::vec2(1.0f)}, Colour::RED);
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

	ImGui::Begin("UI Editor");
	ImGuiID dockspaceID = ImGui::GetID("UIEditorDockspace");
	ImGui::DockSpace(dockspaceID, ImVec2(0,0), ImGuiDockNodeFlags_PassthruCentralNode);
	ImGui::End();

	ImGui::Begin("UI Hierarchy", &open);
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
	

	// UI Inspector
	ImGui::Begin("UI Inspector", &open);

	if (selectedObject)
	{
		ImGui::SeparatorText("UIObject");

		const int BUFFER_SIZE = 100;
		char bufferName[BUFFER_SIZE + 1];
		const std::string& name = selectedObject->GetName();
		for (int i = 0; i < Math::min(BUFFER_SIZE, name.size()); ++i)
			bufferName[i] = name[i];
		bufferName[Math::min(BUFFER_SIZE, name.size())] = '\0';

		if (ImGui::InputText("Name", bufferName, BUFFER_SIZE, ImGuiInputTextFlags_EnterReturnsTrue))
			selectedObject->SetName(bufferName);

		AnchorPoint anchorPoint = selectedObject->GetAnchorPoint();
		ImGui::Text("AnchorPoint");
		ImGui::SameLine();
		if (ImGui::Button(anchorPoint.ToString().c_str()))
			ImGui::OpenPopup("anchorpoint_popup");
		if (ImGui::BeginPopup("anchorpoint_popup"))
		{
			ImGui::SeparatorText("AnchorPoint");
			for (int i = 0; i < AnchorPoint::MAX_NUMBER; i++)
				if (ImGui::Selectable(AnchorPoint(i).ToString().c_str()))
					selectedObject->SetAnchorPoint(AnchorPoint(i));
			ImGui::EndPopup();
		}
		glm::vec2 relativePos = selectedObject->GetRelativePos();
		if (ImGui::DragFloat2("Relative Position", (float*)(&relativePos)))
			selectedObject->SetRelativePos(relativePos);

		glm::vec2 renderPos = selectedObject->GetRenderPos();
		ImGui::InputFloat2("Render Position", (float*)(&renderPos), "%.2f", ImGuiInputTextFlags_ReadOnly);

		glm::vec4 backgroundCol = selectedObject->backgroundColour;
		if (ImGui::ColorEdit4("Background", (float*)(&backgroundCol)))
			selectedObject->SetBackgroundColour(backgroundCol);

		glm::vec2 size = selectedObject->GetSize();
		if (ImGui::DragFloat2("Size", (float*)(&size)))
			selectedObject->SetSize(size);

		ImGui::SeparatorText("UIObject");
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

void UIManager::SetEditorModeActive(bool active)
{
	editorModeActive = true;
}




