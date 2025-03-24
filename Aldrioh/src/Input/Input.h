#pragma once

#include <GLFW/glfw3.h>
#include "InputCode.h"

namespace Input
{
	bool IsKeyPressed(uint32_t keyCode);
	bool IsMouseButtonPressed(uint32_t button);
	float GetMouseX();
	float GetMouseY();
	glm::vec2 GetMousePosition();
}