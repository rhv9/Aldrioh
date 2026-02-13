#pragma once
#include "Wave.h"

class WaveGroup
{
public:
	WaveGroup(const std::string& name, float durationSeconds) : name(name), durationSeconds(durationSeconds) {}
	WaveGroup(const std::string& name, float durationSeconds, std::vector<IWaveLogic*> logicVector);

	void OnUpdate(Timestep ts);
	void OnCreate();
	void OnFinish();

	std::string name;
	float durationSeconds;
	
	static constexpr int MAX_WAVES = 16;
	int addedIndex = 0;
	std::array<std::unique_ptr<IWaveLogic>, MAX_WAVES> waveLogicArray;
};
