#include <pch.h>
#include "WaveGroup.h"

WaveGroup::WaveGroup(const std::string& name, float durationSeconds, std::vector<IWaveLogic*> logicVector) : name(name), durationSeconds(durationSeconds)
{
	for (int i = 0; i < logicVector.size(); ++i)
	{
		waveLogicArray[addedIndex++] = std::unique_ptr<IWaveLogic>(logicVector[i]);
	}
}

void WaveGroup::OnUpdate(Timestep ts)
{
	for (int i = 0; i < addedIndex; ++i)
		waveLogicArray[i]->OnUpdate(ts);
}

void WaveGroup::OnCreate()
{
	for (int i = 0; i < addedIndex; ++i)
		waveLogicArray[i]->OnCreate();
}
void WaveGroup::OnFinish()
{
	for (int i = 0; i < addedIndex; ++i)
		waveLogicArray[i]->OnFinish();
}