#include <pch.h>
#include "ImGuiWindows.h"
#include <imgui.h>
#include <Core/Platform.h>
#include <Game.h>

#include <Graphics/Renderer.h>
#include <Debug/Statistics.h>
#include <Debug/GameDebugState.h>

void GameImGuiWindows::ShowGameInfo()
{
	if (ImGui::CollapsingHeader("Game"))
	{
		auto engineStats = Statistics::EngineStats::GetStats();
		ImGui::Text("frame time: %.2f (%dfps)", engineStats.delta.GetMilliSeconds(), engineStats.fps);
		ImGui::Text("Elapsed time: %.2f", Platform::GetElapsedTime());
	}
}

void GameImGuiWindows::ShowRendererInfo()
{
	if (ImGui::CollapsingHeader("Renderer"))
	{
		bool renderDepth = Renderer::IsRenderDepth();
		if (ImGui::Checkbox("Render depth", &renderDepth))
			Renderer::SetRenderDepthOnly(renderDepth);
		ImGui::Checkbox("Show Collision Box", &GameDebugState::showCollisionBox);

		if (ImGui::TreeNode("RenderQueue"))
		{
			auto rqStats = Statistics::RenderQueueStats::GetStats();
			ImGui::Text("Render Counts");
			for (int i = 0; i < rqStats.layerRenderCounts.size(); ++i)
				ImGui::Text("  Layer %d: %d", i, rqStats.layerRenderCounts[i]);
			ImGui::Text("  Total: %d", rqStats.renderCount);
			ImGui::TreePop();
		}
	}
}

void GameImGuiWindows::ShowImGuiInfo()
{
	auto& game = Game::Instance();
	ImGuiIO& io = ImGui::GetIO();

	if (ImGui::CollapsingHeader("ImGui"))
	{
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
	}
}

