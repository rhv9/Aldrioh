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

float Math::Random::normalized()
{
	return glm::linearRand(0.0f, 1.0f);
}



float Math::sinRad(float val)
{
	return sinf(val);
}

float Math::cosRad(float val)
{
	return cosf(val);
}

float Math::arcTan(float x)
{
	return atanf(x);
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

float Math::angleBetween2d(const glm::vec3& p1, const glm::vec3& p2)
{
	float dx = p2.x - p1.x;
	float dy = p2.y - p1.y;
	return atan2f(dy, dx) - Math::PI / 2.0f;
}

float Math::angle(const glm::vec2& pos)
{
	return atan2f(pos.y, pos.x) - Math::PI / 2.0f;
}

float Math::dist(const glm::vec2& left, const glm::vec2& right)
{
	return glm::length(left - right);
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

float Math::gradient(const glm::vec2& p1, const glm::vec2& p2)
{
	float dy = p2.y - p1.y;
	float dx = p2.x - p1.x;
	if (dx == 0) return 0;
	return dy / dx;
}

glm::vec2 Math::bezier3(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, float t)
{
	// Q0 = (1-t)p0 + tp1
	// Q1 = (1-t)p1 + tp2
	// (1-t)Q0 + tQ1
	return  (1 - t) * ((1 - t) * p0 + t * p1) + t * ((1 - t) * p1 + t * p2);
}

float Math::abs(float val)
{
	return (val < 0) ? -val : val;
}

float Math::sign(float val)
{
	return val < 0 ? -1 : 1;
}

float Math::floatMod(float lhs, float rhs)
{
	return fmodf(lhs, rhs);
}

float Math::floatModNegative(float lhs, float rhs)
{
	return fmodf(fmodf(lhs, rhs) + rhs, rhs);
}

int Math::ffloor(float val)
{
	return floorf(val);
}

float Math::min(float x, float y)
{
	return x < y ? x : y;
}

float Math::max(float x, float y)
{
	return x > y ? x : y;
}

float Math::minAndMax(float lhs, float val, float rhs)
{
	if (val < lhs)
		return lhs;
	else if (val > rhs)
		return rhs;
	else
		return val;
}
