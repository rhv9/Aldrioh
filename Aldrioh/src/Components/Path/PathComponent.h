#pragma once
#include <array>
#include <glm/glm.hpp>
#include "Path.h"

struct PathComponent
{
	Path path;
	glm::vec2 currentPosition{ 0 };
	float time = 0;
	int currentPathIndex = 0;

	PathComponent() = default;
	PathComponent(const PathComponent&) = default;
};


