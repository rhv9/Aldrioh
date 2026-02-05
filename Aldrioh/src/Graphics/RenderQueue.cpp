#include <pch.h>
#include "RenderQueue.h"
#include "Renderer.h"
#include <Game/SpriteCollection.h>
#include <Debug/Statistics.h>

#include <UI/Font.h>

RenderQueue::RenderQueueData RenderQueue::rqData;

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

void RenderQueue::EnQueue(RenderLayer layer, const glm::vec3& pos, int spriteId, const glm::vec4& colour, const glm::vec2& scale, float rotation, float flags)
{
	EnQueue(layer, pos, Sprites::get(spriteId), colour, scale, rotation, flags);
}

void RenderQueue::EnQueueText(RenderLayer layer, const glm::vec3& pos, const FontStyle* fontStyle, const std::string& text, const glm::vec4& colour, const glm::vec2& scale)
{
	glm::vec3 absolutePos = pos;
	glm::vec2 absoluteSize = glm::vec2(scale.x, scale.y);

	for (char c : text)
	{
		if (c != ' ')
		{
			SubTexture* charSubTexture = fontStyle->font->GetCharSubTexture(c);
			EnQueue(layer, absolutePos, charSubTexture, colour, absoluteSize, 0.0f, 1.0f);
		}
		absolutePos.x += fontStyle->charSpacingPercent * scale.x;
	}
}

void RenderQueue::EnQueue(RenderLayer layer, const glm::vec3& pos, SubTexture* subTexture, const glm::vec4& colour, const glm::vec2& scale, float rotation, float flags)
{
	ASSERT(layer >= 0 && layer < RenderQueue::COUNT_RENDERLAYERS, "Layer does not exist!");

	if (rqData.layers[layer].count >= RenderQueueData::MAX_RENDEROBJECTS)
	{
		FlushAndReset();
	}

	rqData.layers[layer].ptr->pos = pos;
	rqData.layers[layer].ptr->subTexturePtr = subTexture;
	rqData.layers[layer].ptr->scale = scale;
	rqData.layers[layer].ptr->rotation = rotation;
	rqData.layers[layer].ptr->colour = colour;
	rqData.layers[layer].ptr->flags = flags;

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
			Renderer::DrawQuad(renderObj.pos, renderObj.subTexturePtr, renderObj.scale, renderObj.colour, renderObj.rotation, renderObj.flags);
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
	for (int i = 0; i < RenderQueue::COUNT_RENDERLAYERS; ++i)
	{
		delete[] rqData.layers[i].basePtr;
	}
}
