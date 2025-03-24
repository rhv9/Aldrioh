#include "pch.h"
#include "Math.h"

uint32_t Math::Random::linearInt(int min, int max)
{
	return (uint32_t) glm::linearRand(min, max);
}
