#include "pch.h"
#include "Math.h"

uint32_t Math::Random::linearInt(int min, int max)
{
	return (uint32_t) glm::linearRand(min, max);
}

float Math::Random::linearFloat(float min, float max)
{
	return glm::linearRand(min, max);
}
