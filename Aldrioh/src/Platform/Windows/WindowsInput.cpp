#include "pch.h"
#include "Game.h"
#include "Input/Input.h"
#include "WindowsWindow.h"


bool Input::IsKeyPressed(uint32_t keyCode)
{
    int status = glfwGetKey((GLFWwindow*)Game::Instance().GetWindow()->GetNativeWindow(), static_cast<int>(keyCode));
    return (status == GLFW_PRESS || status == GLFW_REPEAT) && !Game::Instance().isEventsBlocked();
}

bool Input::IsMouseButtonPressed(uint32_t button)
{
    int status = glfwGetMouseButton((GLFWwindow*)Game::Instance().GetWindow()->GetNativeWindow(), button);
    return status == GLFW_PRESS && !Game::Instance().isEventsBlocked();
}

float Input::GetMouseX()
{
    return GetMousePosition().x;
}

float Input::GetMouseY()
{
    return GetMousePosition().y;
}

glm::vec2 Input::GetMousePosition()
{
    double xpos, ypos;
    glfwGetCursorPos((GLFWwindow*)Game::Instance().GetWindow()->GetNativeWindow(), &xpos, &ypos);
    return { (float)xpos, (float)ypos };
}