#include <pch.h>
#include "UIImage.h"

#include <Graphics/Renderer.h>
#include <imgui.h>

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
}
