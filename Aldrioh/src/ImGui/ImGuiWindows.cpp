#include <pch.h>
#include "ImGuiWindows.h"
#include <imgui.h>
#include <Core/Platform.h>
#include <Game.h>

#include <Graphics/Renderer.h>
#include <Debug/Statistics.h>
#include <Debug/GameDebugState.h>
#include <Input/Input.h>

#include <Audio/SoundManager.h>
void GameImGuiWindows::ShowGameInfo()
{
	if (ImGui::CollapsingHeader("Game"))
	{
		auto engineStats = Statistics::EngineStats::GetStats();
		ImGui::Text("frame time: %.2f (%dfps)", engineStats.delta.GetMilliSeconds(), engineStats.fps);
		ImGui::Text("Elapsed time: %.2f", Platform::GetElapsedTime());
		ImGui::Text("Layer Count: %d", engineStats.layerCount);
	}

	if (ImGui::CollapsingHeader("Audio"))
	{
		float audio = SoundManager::GetVolume(SoundCategory::MASTER);
		if (ImGui::SliderFloat("Master", &audio, 0, 1))
			SoundManager::SetVolume(SoundCategory::MASTER, audio);

		audio = SoundManager::GetVolume(SoundCategory::SFX);
		if (ImGui::SliderFloat("SFX", &audio, 0, 1))
			SoundManager::SetVolume(SoundCategory::SFX, audio);
	}

}

void GameImGuiWindows::ShowRendererInfo()
{
	if (ImGui::CollapsingHeader("Renderer"))
	{
		auto& game = Game::Instance();
		bool vsync = game.GetWindow()->GetVsync();
		if (ImGui::Checkbox("vsync", &vsync))
			game.GetWindow()->SetVsync(vsync);
		ImGui::Checkbox("Show Collision Box", &GameDebugState::showCollisionBox);

		if (ImGui::TreeNode("Main Renderer"))
		{
			auto& renderStats = Statistics::RendererStats::GetStats();
			ImGui::Text("Draw Calls: %d", renderStats.drawCalls);
			ImGui::Text("Draw Quads: %d", renderStats.batchQuads);
			ImGui::Text("Draw Vertices: %d", renderStats.batchVertices);
			ImGui::Text("Draw Indices: %d", renderStats.batchIndices);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("RenderQueue"))
		{
			auto rqStats = Statistics::RendererStats::GetStats();
			ImGui::Text("Render Counts");
			for (int i = 0; i < rqStats.renderQueueLayerRenderCounts.size(); ++i)
				ImGui::Text("  Layer %d: %d", i, rqStats.renderQueueLayerRenderCounts[i]);
			ImGui::Text("  Total: %d", rqStats.renderQueueCount);
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
		bool navActive = io.NavActive;
		ImGui::Checkbox("ImGui Nav Active", &navActive);
		bool wantCaptureKeyboard = io.WantCaptureKeyboard;
		ImGui::Checkbox("ImGui capture keyboard", &wantCaptureKeyboard);
		bool wantCaptureMouse = io.WantCaptureMouse;
		ImGui::Checkbox("ImGui capture mouse", &wantCaptureMouse);
	}
}

void GameImGuiWindows::ShowWindowInfo()
{
	auto& window = Game::Instance().GetWindow();
	if (ImGui::CollapsingHeader("Window"))
	{
		ImGui::Text("Size: (%d, %d)", window->GetWidth(), window->GetHeight());
		ImGui::Text("Pos: (%.2f, %.2f)", window->GetPos().x, window->GetPos().y);
		ImGui::Text("Aspect Ratio: %.4f", window->GetAspectRatio());
		ImGui::Text("Mouse pos: (%.1f, %.1f)", Input::GetMouseX(), Input::GetMouseY());
	}
}

