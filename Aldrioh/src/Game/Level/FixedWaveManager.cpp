#include <pch.h>
#include "FixedWaveManager.h"
#include <Scene/Entity.h>
#include "Level.h"

#include <Game/Debug/GameDebugState.h>
#include <imgui.h>

FixedWaveManager::FixedWaveManager(Scene& scene, Level& level) : scene(scene), level(level)
{
}

void FixedWaveManager::InitWaveConfig()
{
	SpawnerWaveConfig wave1Config;
	wave1Config.cooldownMin = 0.1f;
	wave1Config.cooldownMax = 0.5f;
	wave1Config.maxEntities = 15.0f;
	waveQueue.emplace_back("Wave 1", 60,
		std::vector<IWaveLogic*> {
		new SpawnerWave(level, wave1Config, EnemyEntityTypes::Drone_Normal),
	});

	waveQueue.emplace_back("Wave 2", 60,
		std::vector<IWaveLogic*> {
		new SpawnerWave(level, wave1Config, EnemyEntityTypes::Drone_Normal),
		new SpawnerWave(level, wave1Config, EnemyEntityTypes::Drone_Tank)
	});
	waveQueue.emplace_back("Wave 3", 15);

	finishTime = waveQueue.front().durationSeconds;
}

void FixedWaveManager::OnUpdate(Timestep ts)
{
	// Debugging
	if (!GameDebugState::level_spawnEntites)
		return;

	if (waveIndex < waveQueue.size())
	{
		// Updating waveIndex
		float levelTime = level.GetElapsedTime();
		if (levelTime >= finishTime)
		{
			waveQueue[waveIndex].OnFinish();
			++waveIndex;
			if (waveIndex >= waveQueue.size())
				return;
			finishTime += waveQueue[waveIndex].durationSeconds;
			waveQueue[waveIndex].OnCreate();
		}
		waveQueue[waveIndex].OnUpdate(ts);
	}
}

void FixedWaveManager::SetWave(int index)
{
	finishTime = 0;
	for (int i = 0; i < index + 1; ++i)
	{
		finishTime += waveQueue[i].durationSeconds;
	}
	waveIndex = index;
}

void FixedWaveManager::OnImGuiDebugging()
{
	ImGui::Checkbox("Spawn Enemies", &GameDebugState::level_spawnEntites);
	for (int i = 0; i < waveQueue.size(); ++i)
	{
		WaveGroup& wave = waveQueue[i];
		ImGui::PushID(i);

		ImGui::PushStyleColor(ImGuiCol_Text, i == waveIndex ? ImVec4{ 1.0f, 1.0f, 0.0f, 1.0f } : ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f });
		ImGui::Text("%d:%s", i, wave.name.c_str());
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::DragFloat("", &wave.durationSeconds);
		ImGui::PopID();
	}

}
