#include <pch.h>
#include "RenderQueue.h"
#include "Renderer.h"
#include <Game/SpriteCollection.h>

struct RenderLayerData
{
	SpriteRenderObject* basePtr = nullptr;
	SpriteRenderObject* ptr = nullptr;
	uint32_t count = 0;
};

struct RenderQueueData
{
	const static int MAX_RENDEROBJECTS = 5000;
	const static int COUNT_RENDERLAYERS = 5;

	uint32_t renderCount = 0;

	std::array<RenderLayerData, COUNT_RENDERLAYERS> layers;
};


static RenderQueueData rqData;

void RenderQueue::Init()
{
	static bool initialized = false;
	LOG_CORE_INFO("RenderQueue::Init()");

	if (initialized)
	{
		LOG_CORE_CRITICAL("Initialising RenderQueue twice!");
		initialized = true;
	}

	for (RenderLayerData& rlData : rqData.layers)
	{
		rlData.basePtr = new SpriteRenderObject[RenderQueueData::MAX_RENDEROBJECTS];
	}

	Reset();
}

void RenderQueue::Reset()
{
	for (RenderLayerData& rlData : rqData.layers)
	{
		rlData.ptr = rlData.basePtr;
		rlData.count = 0;
	}
	rqData.renderCount = 0;
}

void RenderQueue::EnQueue(RenderLayer layer, const glm::vec3& pos, int spriteId, const glm::vec2& scale)
{
	ASSERT(layer >= 0 && layer < RenderQueueData::COUNT_RENDERLAYERS, "Layer does not exist!");

	if (rqData.layers[layer].count >= RenderQueueData::MAX_RENDEROBJECTS)
	{
		FlushAndReset();
		return;
	}

	rqData.layers[layer].ptr->pos = pos;
	rqData.layers[layer].ptr->spriteId = spriteId;
	rqData.layers[layer].ptr->scale = scale;

	++rqData.layers[layer].ptr;
	++rqData.layers[layer].count;
	++rqData.renderCount;
}


void RenderQueue::Flush()
{
	for (RenderLayerData& rlData : rqData.layers)
	{
		for (int i = 0; i < rlData.count; ++i)
		{
			SpriteRenderObject& renderObj = rlData.basePtr[i];
			Renderer::DrawQuad(renderObj.pos, Sprites::get(renderObj.spriteId), renderObj.scale);
		}
	}
}

void RenderQueue::FlushAndReset()
{
	Flush();
	Reset();
}

void RenderQueue::Shutdown()
{
	for (int i = 0; i < RenderQueueData::COUNT_RENDERLAYERS; ++i)
	{
		delete[] rqData.layers[i].basePtr;
	}
}
