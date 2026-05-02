#pragma once
#include <Math/Math.h>



struct RocketShooterComponent
{
	enum class State : uint8_t
	{
		NOT_ACTIVE,
		ACTIVE,
		EXHAUSTED,
	};

	float timer = 1.0f;
	float elapsedTime = 0.0f;
	float angleFacing = 0.0f;
	float targetAngle = 0.0f;
	float rotationSpeed = Math::PI;
	constexpr static float ROTATION_OFFSET = -Math::PI / 4.0f;
	State state = State::NOT_ACTIVE;

	RocketShooterComponent() {}
	RocketShooterComponent(const RocketShooterComponent&) = default;
};