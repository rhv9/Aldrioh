#include "pch.h"
#include "FreeCameraController.h"

#include "Graphics/Camera.h"

#include "Input/Input.h"
#include "Game.h"


FreeCameraController::FreeCameraController(const float aspectRatio, const float zoomLevel)
	: CameraController(aspectRatio, zoomLevel) 
{
	Game::Instance().GetWindow()->MouseScrolledEventHandler += EVENT_BIND_MEMBER_FUNCTION(FreeCameraController::OnMouseScrollCallback);
	Game::Instance().GetWindow()->WindowResizeEventHandler += EVENT_BIND_MEMBER_FUNCTION(FreeCameraController::OnWindowResizeCallback);

	// Camera dragging
	Game::Instance().GetWindow()->MouseButtonPressedEventHandler += EVENT_BIND_MEMBER_FUNCTION(FreeCameraController::OnMousePressedCallback);
	Game::Instance().GetWindow()->MouseButtonReleasedEventHandler += EVENT_BIND_MEMBER_FUNCTION(FreeCameraController::OnMouseReleasedCallback);
	Game::Instance().GetWindow()->MouseMoveEventHandler += EVENT_BIND_MEMBER_FUNCTION(FreeCameraController::OnMouseMoveCallback);
}


void FreeCameraController::OnUpdate(Timestep ts)
{
	glm::vec2 move{ 0.0f };

	if (Input::IsKeyPressed(Input::KEY_W))
		move.y += zoomLevel * ts;
	if (Input::IsKeyPressed(Input::KEY_S))
		move.y -= zoomLevel * ts;
	if (Input::IsKeyPressed(Input::KEY_D))
		move.x += zoomLevel * ts;
	if (Input::IsKeyPressed(Input::KEY_A))
		move.x -= zoomLevel * ts;

	if (Input::IsKeyPressed(Input::KEY_LEFT))
		rotation -= rotationSpeed * ts;
	if (Input::IsKeyPressed(Input::KEY_RIGHT))
		rotation += rotationSpeed * ts;

	this->SetPosition(glm::vec3{ m_Position + move, 0.0f });

	if (move != glm::zero<glm::vec2>())
	{
		initialCameraPos += move;
	}
}




void FreeCameraController::OnMousePressedCallback(MouseButtonPressedEventArg& e)
{
	if (e.Button == Input::MOUSE_BUTTON_LEFT)
	{
		mousePressedPoint = Input::GetMousePosition();
		initialCameraPos = GetPosition();
		mouseHeld = true;
	}
}

void FreeCameraController::OnMouseReleasedCallback(MouseButtonReleasedEventArg& e)
{
	mouseHeld = false;
}

void FreeCameraController::OnMouseMoveCallback(MouseMoveEventArg& e)
{
	uint32_t windowWidth = Game::Instance().GetWindow()->GetWidth();
	uint32_t windowHeight = Game::Instance().GetWindow()->GetHeight();

	//LOG_CORE_TRACE("Window width, height: ({},{})", windowWidth, windowHeight);
	//LOG_CORE_TRACE("Bounds: ({},{})", bounds.GetWidth(), bounds.GetHeight());
	//LOG_CORE_TRACE("MousePos: ({},{})", Input::GetMousePosition().x, Input::GetMousePosition().y);


	if (mouseHeld)
	{
		glm::vec2 mousePos = Input::GetMousePosition();

		glm::vec2 offset = mousePos - mousePressedPoint;

		glm::vec2 offsetPercentage;
		offsetPercentage.x = offset.x / windowWidth;
		offsetPercentage.y = offset.y / windowHeight;

		//offset.x = -offset.x;
		glm::vec2 result;

		result.x = offsetPercentage.x * bounds.GetWidth();
		result.y = offsetPercentage.y * bounds.GetHeight();

		result.x = -result.x;
		SetPosition(initialCameraPos + result);
	}
}

void FreeCameraController::OnMouseScrollCallback(MouseScrolledEventArg& e)
{
	SetZoomLevel(zoomLevel + -e.YOffset * 0.1f * zoomLevel);
	LOG_CORE_INFO("Zoom Level: {}", zoomLevel);
}

void FreeCameraController::OnWindowResizeCallback(WindowResizeEventArg& e)
{
	SetAspectRatio((float)e.Width / (float)e.Height);
}

