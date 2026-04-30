#pragma once
#include <Math/Math.h>

struct RocketShooterComponent
{
	float timer = 1.0f;
	float elapsedTime = 0.0f;
	float angleFacing = 0.0f;
	float rotationSpeed = Math::PI;
	constexpr static float ROTATION_OFFSET = -Math::PI / 4.0f;
	bool active = false;

	RocketShooterComponent() {}
	RocketShooterComponent(const RocketShooterComponent&) = default;
};