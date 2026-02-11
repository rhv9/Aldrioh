#include "pch.h"
#include "CameraController.h"

#include "Graphics/Camera.h"

#include "Input/Input.h"
#include "Game.h"


CameraController::CameraController(const float aspectRatio, const float zoomLevel)
	: aspectRatio(aspectRatio), zoomLevel(zoomLevel), bounds({ -aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel }), camera(bounds.Left, bounds.Right, bounds.Bottom, bounds.Top)
{
	SetAspectRatio((float)Game::Instance().GetWindow()->GetWidth() / (float)Game::Instance().GetWindow()->GetHeight());
}

void CameraController::OnUpdate(Timestep delta)
{
}

void CameraController::SetPosition(const glm::vec2& pos)
{
	m_Position = pos;
	camera.SetPosition({ m_Position , 0.0f });
}

void CameraController::OnResize(uint32_t width, uint32_t height)
{
	SetAspectRatio((float)width / (float)height);
}


void CameraController::OnWindowResizeCallback(WindowResizeEventArg& e)
{
	SetAspectRatio((float)e.Width / (float)e.Height);
}

void CameraController::CalculateView()
{
	float flipY = flipOnYAxis ? -1 : 1;
	bounds = { -aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel * flipY, zoomLevel * flipY};

	camera.SetProjectionMatrix(bounds.Left, bounds.Right, bounds.Bottom, bounds.Top);
}



