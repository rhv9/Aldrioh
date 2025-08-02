#pragma once
#include "RenderObject.h"

enum RenderLayer
{
	ZERO  = 0,
	ONE   = 1,
	TWO   = 2,
	THREE = 3,
	FOUR  = 4,
};

namespace Statistics {
	struct RendererStats;
}

class RenderQueue
{
public:
	static void Init();


	static void Reset();
	static void EnQueue(RenderLayer layer, const glm::vec3& pos, int spriteId, const glm::vec2& scale = {1.0f, 1.0f});
	static void Flush();
	static void FlushAndReset();

	static void Shutdown();

	const static int COUNT_RENDERLAYERS = 5;

private:

	struct RenderLayerData
	{
		SpriteRenderObject* basePtr = nullptr;
		SpriteRenderObject* ptr = nullptr;
		uint32_t count = 0;
	};

	struct RenderQueueData
	{
		const static int MAX_RENDEROBJECTS = 5000;
		uint32_t renderCount = 0;

		std::array<RenderLayerData, RenderQueue::COUNT_RENDERLAYERS> layers;
	};

	static RenderQueueData rqData;

	friend Statistics::RendererStats;

};