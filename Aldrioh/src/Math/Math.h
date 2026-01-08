#pragma once

namespace Math {

	constexpr float PI = 3.14159265358979323846f;

	namespace Random {

		// Min/max inclusive
		uint32_t linearInt(int min, int max);
		// Min/max inclusive
		float linearFloat(float min, float max);
	}

	float abs(float val);
	float sign(float val);

	float min(float x, float y);
	float max(float x, float y);

	float sinRad(float val);
	float cosRad(float val);
	float arcTan(float x);
	float angle(const glm::vec2& pos);

	glm::vec2 angleToNormalizedVector(float angle);

	float degreesToRad(float val);

	glm::vec2 perpendicularClockwise(const glm::vec2& pos);
	glm::vec2 perpendicularCounterClockwise(const glm::vec2& pos);

	float angleBetween(const glm::vec2& p1, const glm::vec2& p2);
	float angleBetween2d(const glm::vec3& p1, const glm::vec3& p2);
	glm::vec2 normalizedDirection(const glm::vec2& origin, const glm::vec2& dest);

	float triangleWave(float t);

	float gradient(const glm::vec2& p1, const glm::vec2& p2);
}