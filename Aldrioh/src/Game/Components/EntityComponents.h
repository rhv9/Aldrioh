#pragma once
#include <Scene/Entity.h>

struct ShipComponent
{
	glm::vec2 directionFacing;

	ShipComponent() = default;
	ShipComponent(const ShipComponent&) = default;
};

