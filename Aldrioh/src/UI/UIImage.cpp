#include <pch.h>
#include "UIImage.h"

#include <Graphics/Renderer.h>
#include <imgui.h>
#include "UIManager.h"

UIImage::UIImage(const std::string& name, const glm::vec2& relativePos, const glm::vec2& size)
	: UIObject(name, relativePos, size)
{
}

void UIImage::OnUpdate(Timestep ts)
{
	UIObject::OnUpdate(ts);
}

void UIImage::OnRender(Timestep ts)
{
	Renderer::UIDrawImage(subTexture, renderPos, size, backgroundColour);
}

void UIImage::ImGuiDraw()
{
	ImGui::SeparatorText("UIImage");

	ImGui::Text("Sprite Size: (%d, %d)", subTexture->width, subTexture->height);

	ImGui::Checkbox("Lock Aspect Ratio", &editorLockSpriteAspectRatio);

	ImGui::SameLine();
	
	if (ImGui::Checkbox("Pixel movement", &editorLockPixelMovement))
	{
		if (editorLockPixelMovement)
			this->GetUIManager()->SetEditorGridSize((float)GetWidth() / subTexture->width);
		else 
			this->GetUIManager()->DisableEditorGridSize();
	}

	glm::vec2 size = GetSize();
	if (ImGui::DragFloat2("Size##2", (float*)(&size)))
	{
		if (editorLockSpriteAspectRatio)
		{
			glm::vec2 oldSize = GetSize();
			if (oldSize.x != size.x)
				size.y = ((float)subTexture->height / subTexture->width) * size.x;
			else
				size.x = ((float)subTexture->width / subTexture->height) * size.y;
		}
		SetSize(size);

		if (editorLockPixelMovement)
			this->GetUIManager()->SetEditorGridSize((float)GetWidth() / subTexture->width);
	}

}
