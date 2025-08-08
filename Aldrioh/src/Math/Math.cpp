#include "pch.h"
#include "Math.h"

#include <cmath>

uint32_t Math::Random::linearInt(int min, int max)
{
	return (uint32_t) glm::linearRand(min, max);
}

float Math::Random::linearFloat(float min, float max)
{
	return glm::linearRand(min, max);
}

float inline Math::sinf(float val)
{
	return sinf(val);
}
