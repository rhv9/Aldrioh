#pragma once

namespace Math {

	constexpr float PI = 3.14159265358979323846f;

	namespace Random {

		uint32_t linearInt(int min, int max);
		float linearFloat(float min, float max);
	}

	float sinRad(float val);
	float cosRad(float val);
	float tan2Rad(float y, float x);

	float degreesToRad(float val);

	float angleBetween(const glm::vec2& p1, const glm::vec2& p2);
	float angleBetweenVec2(const glm::vec3& p1, const glm::vec3& p2);

	float angle(const glm::vec2& pos);

}