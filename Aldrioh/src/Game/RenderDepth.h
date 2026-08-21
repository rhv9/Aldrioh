#pragma once
#include <Graphics/RenderLayer.h>

class RenderDepth
{
public:
	static float TILE;
	static float ENTITY;
	static float PARTICLE;
	static float COLLECTABLES;
	static float DEBUG_TOP;
};

namespace GameRenderLayers
{
	static const RenderLayer ENTITIES = RenderLayer::TWO;
	static const RenderLayer PARTICLES = RenderLayer::ONE;
	static const RenderLayer NUMBER_PARTICLES = RenderLayer::THREE;
	static const RenderLayer COLLISION_BOX = RenderLayer::FOUR;
}