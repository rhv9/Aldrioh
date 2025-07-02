#pragma once

namespace Statistics {

	struct RenderQueueStats 
	{
		static RenderQueueStats GetStats();

		uint32_t renderCount;
		std::array<uint32_t, 5> layerRenderCounts;
	};

	struct EngineStats
	{

	};
}