#include "pch.h"
#include "Math.h"

#include <cmath>

uint32_t Math::Random::linearInt(int min, int max)
{
	return (uint32_t)glm::linearRand(min, max);
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

float Math::tan2Rad(float y, float x)
{
	return atan2f(y, x);
}

float Math::degreesToRad(float degrees)
{
	return degrees * (PI / 180.0f);
}

glm::vec2 Math::perpendicularClockwise(const glm::vec2& pos)
{
	return glm::vec2{ pos.y, -pos.x };
}

glm::vec2 Math::perpendicularCounterClockwise(const glm::vec2& pos)
{
	return glm::vec2{ -pos.y, pos.x };
}

float Math::angleBetween(const glm::vec2& p1, const glm::vec2& p2)
{
	float dx = p2.x - p1.x;
	float dy = p2.y - p1.y;
	return atan2f(dy, dx) - Math::PI / 2.0f;
}

float Math::angleBetweenVec2(const glm::vec3& p1, const glm::vec3& p2)
{
	float dx = p2.x - p1.x;
	float dy = p2.y - p1.y;
	return atan2f(dy, dx) - Math::PI / 2.0f;
}

float Math::angle(const glm::vec2& pos)
{
	return atan2f(pos.y, pos.x) - Math::PI / 2.0f;
}

glm::vec2 Math::angleToNormalizedVector(float angle)
{
	return glm::vec2(cosRad(angle), sinRad(angle));
}

glm::vec2 Math::normalizedDirection(const glm::vec2& origin, const glm::vec2& dest)
{
	return glm::normalize(dest - origin);
}

float Math::triangleWave(float t)
{
	float x = (2.0f * t - PI) / PI;
	return Math::abs(fmodf(x, 4.0f) - 2.0f) - 1.0f;
}

float Math::abs(float val)
{
	return (val < 0) ? -val : val;
}
