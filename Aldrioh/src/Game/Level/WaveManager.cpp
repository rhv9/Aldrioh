#include <pch.h>
#include "WaveManager.h"
#include <Scene/Entity.h>
#include "Level.h"

#include <Game/Debug/GameDebugState.h>
#include <imgui.h>

WaveManager::WaveManager(Scene& scene, Level& level) : scene(scene), level(level)
{
}

void WaveManager::InitWaveConfig()
{
	waveQueue.push_back(Wave{ "Wave 1", 10 });
	waveQueue.push_back(Wave{ "Wave 2", 5 });
	waveQueue.push_back(Wave{ "Wave 3", 15 });

	finishTime = waveQueue.front().durationSeconds;
}

void WaveManager::OnUpdate(Timestep ts)
{
	// Debugging
	if (!GameDebugState::level_spawnEntites)
		return;

	if (!waveQueue.empty())
	{
		// Updating waveIndex
		float levelTime = level.GetElapsedTime();
		if (levelTime >= finishTime)
		{
			++waveIndex;
			if (waveIndex >= waveQueue.size())
				return;
			finishTime += waveQueue[waveIndex].durationSeconds;
		}


	}
}

void WaveManager::SetWave(int index)
{
	finishTime = 0;
	for (int i = 0; i < index + 1; ++i)
	{
		finishTime += waveQueue[i].durationSeconds;
	}
	waveIndex = index;
}

void WaveManager::OnImGuiDebugging()
{
	ImGui::Checkbox("Spawn Enemies", &GameDebugState::level_spawnEntites);
	for (int i = 0; i < waveQueue.size(); ++i)
	{
		Wave& wave = waveQueue[i];
		ImGui::PushID(i);
		
		ImGui::PushStyleColor(ImGuiCol_Text, i == waveIndex ? ImVec4{1.0f, 1.0f, 0.0f, 1.0f} : ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f });
		ImGui::Text("%d:%s", i, wave.name.c_str());
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::DragFloat("", &wave.durationSeconds);
		ImGui::PopID();
	}

}
