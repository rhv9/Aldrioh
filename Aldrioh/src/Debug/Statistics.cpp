#include "pch.h"
#include "Statistics.h"
#include <Graphics/RenderQueue.h>

#include <Game.h>

const Statistics::RenderQueueStats Statistics::RenderQueueStats::GetStats()
{
	std::array<uint32_t, 5> arr;
	for (int i = 0; i < 5; ++i)
		arr[i] = RenderQueue::rqData.layers[i].count;
	RenderQueueStats stats { RenderQueue::rqData.renderCount , arr };
	return stats;
}

const Statistics::EngineStats Statistics::EngineStats::GetStats()
{
	auto& gameStats = Game::Instance().gameStats;
	return EngineStats(Game::Instance().delta, gameStats.fpsCounter, gameStats.fps);
}
