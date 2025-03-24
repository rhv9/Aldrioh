#include "pch.h"
#include "Core/Platform.h"
#include "GLFW/glfw3.h"

float Platform::GetElapsedTime()
{
	return (float)glfwGetTime();
}

