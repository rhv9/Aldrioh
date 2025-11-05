#pragma once

namespace Math {

	constexpr float PI = 3.14159265358979323846f;

	namespace Random {

		uint32_t linearInt(int min, int max);
		float linearFloat(float min, float max);
	}

	float abs(float val);

	float sinRad(float val);
	float cosRad(float val);
	float tan2Rad(float y, float x);
	float angle(const glm::vec2& pos);

	glm::vec2 angleToNormalizedVector(float angle);

	float degreesToRad(float val);

	glm::vec2 perpendicularClockwise(const glm::vec2& pos);
	glm::vec2 perpendicularCounterClockwise(const glm::vec2& pos);

	float angleBetween(const glm::vec2& p1, const glm::vec2& p2);
	float angleBetweenVec2(const glm::vec3& p1, const glm::vec3& p2);
	glm::vec2 normalizedDirection(const glm::vec2& origin, const glm::vec2& dest);

	float triangleWave(float t);
}