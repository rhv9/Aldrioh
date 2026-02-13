#pragma once
#include "WaveGroup.h"

class Level;
class Scene;

class FixedWaveManager
{
public:
	FixedWaveManager(Scene& scene, Level& level);

	void InitWaveConfig();
	void OnUpdate(Timestep ts);

	void SetWave(int index);
	const std::vector<WaveGroup>& GetWaveQueue() const { return waveQueue; }
	int GetWaveIndex() const { return waveIndex; }
	float GetFinishTime() const { return finishTime; }

	void OnImGuiDebugging();

private:
	Scene& scene;
	Level& level;

	float finishTime = 0.0f;
	int waveIndex = 0;
	std::vector<WaveGroup> waveQueue;

	
};