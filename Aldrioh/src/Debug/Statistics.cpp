#include "pch.h"
#include "Statistics.h"
#include <Graphics/RenderQueue.h>

#include <Game.h>

void Statistics::ResetStats()
{
	RendererStats::Reset();
}

Statistics::RendererStats Statistics::RendererStats::stats;

Statistics::RendererStats& Statistics::RendererStats::GetUnderlyingStats()
{
	for (int i = 0; i < 5; ++i)
		stats.renderQueueLayerRenderCounts[i] = RenderQueue::rqData.layers[i].count;
	stats.renderQueueCount = RenderQueue::rqData.renderCount;
	return stats;
}

void Statistics::RendererStats::Reset()
{
	stats.drawCalls = stats.batchVertices = stats.batchQuads = stats.batchIndices = 0;
}

const Statistics::EngineStats Statistics::EngineStats::GetStats()
{
	auto& gameStats = Game::Instance().gameStats;
	EngineStats stats;
	stats.delta = Game::Instance().delta;
	stats.fps = gameStats.fps;
	stats.fpsCounter = gameStats.fpsCounter;
	stats.updateTicks = gameStats.updateTicks;
	stats.ticksPerSecond = gameStats.ticksPerSecond;
	stats.layerCount = Game::Instance().GetLayerStack().layerVector.size();
	return stats;
}

