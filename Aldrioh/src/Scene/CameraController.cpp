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

void CameraController::SetPosition(const glm::vec2& pos)
{
	m_Position = pos;
	camera.SetPosition({ m_Position , 0.0f });
}

void CameraController::OnResize(uint32_t width, uint32_t height)
{
	SetAspectRatio((float)width / (float)height);
}

void CameraController::CalculateView()
{
	bounds = { -aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel };
	camera.SetProjectionMatrix(bounds.Left + 100, bounds.Right + 100, bounds.Bottom, bounds.Top);
}



