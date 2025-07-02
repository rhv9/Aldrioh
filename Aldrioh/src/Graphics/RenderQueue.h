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


class RenderQueue
{
public:
	static void Init();


	static void Reset();
	static void EnQueue(RenderLayer layer, const glm::vec3& pos, int spriteId, const glm::vec2& scale = {1.0f, 1.0f});
	static void FlushAndReset();

	static void Shutdown();

private:
	static void Flush();

public:

};