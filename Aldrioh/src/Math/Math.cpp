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

float Math::sinRad(float val)
{
	return sinf(val);
}

float Math::cosRad(float val)
{
	return cosf(val);
}

float Math::degreesToRad(float degrees)
{
	return degrees * (PI / 180.0f);
}
