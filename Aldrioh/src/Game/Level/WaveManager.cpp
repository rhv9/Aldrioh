#include <pch.h>
#include "WaveManager.h"
#include <Scene/Entity.h>
#include "Level.h"

WaveManager::WaveManager(Scene& scene, Level& level) : scene(scene), level(level)
{

}

void WaveManager::InitWaveConfig()
{
	waveQueue.push(Wave{ 10 });
	waveQueue.push(Wave{ 5 });
	waveQueue.push(Wave{ 15 });

}

void WaveManager::OnUpdate(Timestep ts)
{
	
}