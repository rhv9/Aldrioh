#pragma once

struct ShipComponent
{
	glm::vec2 directionFacing;

	ShipComponent() = default;
	ShipComponent(const ShipComponent&) = default;
};