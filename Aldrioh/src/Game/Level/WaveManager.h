#pragma once

class Level;
class Scene;

class Wave
{
	float durationSeconds;
};

class WaveManager
{
public:
	WaveManager(Scene& scene, Level& level);

	void InitWaveConfig();
	void OnUpdate(Timestep ts);

private:
	Scene& scene;
	Level& level;

	std::queue<Wave> waveQueue;
};