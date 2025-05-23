#include "pch.h"
#include "GameLayer.h"


#include "Game.h"
#include "Core/Platform.h"
#include "Input/Input.h"
#include "Graphics/Renderer.h"

#include "Game/Level/Level.h"
#include "Game/SpriteCollection.h"
#include "RenderDepth.h"

#include <imgui.h>

GameLayer::GameLayer() {}

void GameLayer::OnBegin()
{
	Sprites::init();
	level = std::make_shared<Level>();
}

void GameLayer::OnUpdate(Timestep delta)
{
	level->OnTick(delta);
	level->OnRender(delta);
}

void GameLayer::OnImGuiRender(Timestep delta)
{
	Game& game = Game::Instance();
	ImGuiIO& io = ImGui::GetIO();

	ImGui::SetNextWindowBgAlpha(0.6f);
	ImGui::Begin("Main Window");

	ImGui::SeparatorText("Game/Window");
	ImGui::Text("frame time: %.2f (%dfps)", delta.GetMilliSeconds(), game.gameStats.fps);
	ImGui::Text("Elapsed time: %.2f", Platform::GetElapsedTime());
	ImGui::Text("Blocking events: %s", ImGui::IsWindowFocused() ? "Yes" : "No");
	bool vsync = game.GetWindow()->GetVsync();
	if (ImGui::Checkbox("vsync", &vsync))
		game.GetWindow()->SetVsync(vsync);
	bool navActive = io.NavActive;
	ImGui::Checkbox("ImGui Nav Active", &navActive);
	bool wantCaptureKeyboard = io.WantCaptureKeyboard;
	ImGui::Checkbox("ImGui capture keyboard", &wantCaptureKeyboard);
	bool wantCaptureMouse = io.WantCaptureMouse;
	ImGui::Checkbox("ImGui capture mouse", &wantCaptureMouse);


	ImGui::SeparatorText("Renderer");
	bool renderDepth = Renderer::IsRenderDepth();
	if (ImGui::Checkbox("Render depth", &renderDepth))
		Renderer::SetRenderDepthOnly(renderDepth);
	ImGui::SliderFloat("Tile", &RenderDepth::TILE, 0.0f, 1.0f);
	ImGui::SliderFloat("Entity", &RenderDepth::ENTITY, 0.0f, 1.0f);

	ImGui::SeparatorText("Input");
	ImGui::Text("Mouse Pos: (%.0f, %.0f)", Input::GetMouseX(), Input::GetMouseY());


	if (ImGui::TreeNode("Window"))
	{
		const glm::vec2& windowPos = Game::Instance().GetWindow()->GetPos();
		ImGui::Text("Size: (%d, %d)", game.GetWindow()->GetWidth(), game.GetWindow()->GetHeight());
		ImGui::Text("Position: (%.0f, %.0f)", windowPos.x, windowPos.y);

		ImGui::TreePop();
	}
	ImGui::End();
	ImGui::SetNextWindowBgAlpha(1.0f);
	level->OnImGuiRender(delta);
}

void GameLayer::OnRemove()
{
}