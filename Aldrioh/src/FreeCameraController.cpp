#include "pch.h"
#include "FreeCameraController.h"

#include "Graphics/Camera.h"

#include "Input/Input.h"
#include "Game.h"


FreeCameraController::FreeCameraController(const float aspectRatio, const float zoomLevel)
	: CameraController(aspectRatio, zoomLevel) 
{
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

void FreeCameraController::OnMouseButtonCallback(MouseButtonEventArg& e)
{
	if (e.IsReleased(Input::MOUSE_BUTTON_LEFT))
		mouseHeld = false;
	else if (e.IsPressed(Input::MOUSE_BUTTON_LEFT))
	{
		mousePressedPoint = Input::GetMousePosition();
		initialCameraPos = GetPosition();
		mouseHeld = true;

		//LOG_CORE_INFO("Intial camera relativePos x:{}, y:{}", initialCameraPos.x, initialCameraPos.y);
		//LOG_CORE_INFO("Intial mousepress relativePos x:{}, y:{}", mousePressedPoint.x, /mousePressedPoint.y);
		//LOG_CORE_INFO("Bounds width:{}, height:{}", bounds.GetWidth(), bounds.GetHeight());
	}
}

void FreeCameraController::OnMouseMoveCallback(MouseMoveEventArg& e)
{
	uint32_t windowWidth = Game::Instance().GetWindow()->GetWidth();
	uint32_t windowHeight = Game::Instance().GetWindow()->GetHeight();

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

		SetPosition(initialCameraPos - result);
	}
}

void FreeCameraController::OnMouseScrollCallback(MouseScrolledEventArg& e)
{
	SetZoomLevel(zoomLevel + -e.YOffset * 0.1f * zoomLevel);
	LOG_CORE_INFO("Zoom Level: {}", zoomLevel);
}
