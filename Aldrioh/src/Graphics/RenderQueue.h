#pragma once
#include "RenderObject.h"
#include <Game/SpriteCollection.h>

struct FontStyle;

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
	static void EnQueue(RenderLayer layer, const glm::vec3& pos, SubTexture* subTexture, const glm::vec4& colour = glm::vec4(1.0f), const glm::vec2& scale = { 1.0f, 1.0f }, float rotation = 0.0f, float flags = 0);
	static void EnQueue(RenderLayer layer, const glm::vec3& pos, spriteid_t spriteId, const glm::vec4& colour = glm::vec4(1.0f), const glm::vec2& scale = {1.0f, 1.0f}, float rotation = 0.0f, float flags = 0);
	static void EnQueueText(RenderLayer layer, const glm::vec3& pos, const FontStyle* fontStyle, const std::string& text, const glm::vec4& colour = glm::vec4(1.0f), const glm::vec2& scale = { 1.0f, 1.0f });

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