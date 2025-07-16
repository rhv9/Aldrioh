#pragma once

namespace Statistics {

	struct RenderQueueStats
	{
		static const RenderQueueStats GetStats();

		uint32_t renderCount;
		std::array<uint32_t, 5> layerRenderCounts;
	};

	struct EngineStats
	{
		static const EngineStats GetStats();

		Timestep delta;
		int fpsCounter = 0;
		int fps = 0;
	};
}