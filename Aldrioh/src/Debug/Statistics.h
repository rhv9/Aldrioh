#pragma once

namespace Statistics {
	
	extern void ResetStats();

	struct RendererStats
	{
		static const RendererStats& GetStats() { return GetUnderlyingStats(); }
		static void Reset();
		static RendererStats stats;

		uint32_t renderQueueCount;
		std::array<uint32_t, 5> renderQueueLayerRenderCounts;

		uint32_t drawCalls;
		uint32_t batchVertices, batchIndices, batchQuads;

	private:
		static RendererStats& GetUnderlyingStats();

		friend class Renderer;
	};

	struct EngineStats
	{
		static const EngineStats GetStats();

		Timestep delta = 0;
		int fpsCounter = 0;
		int fps = 0;
		int updateTicks = 0;
		int ticksPerSecond = 0;

		int layerCount = 0;
	};
}