#include "pch.h"
#include "FreeRoamEntityCameraController.h"

#include "Game.h"
#include "Input/Input.h"

#include "Scene/Components.h"

FreeRoamEntityCameraController::FreeRoamEntityCameraController(const float aspectRatio, const float zoomLevel)
	: CameraController(aspectRatio, zoomLevel)
{
	Game::Instance().GetWindow()->MouseScrolledEventHandler += EVENT_BIND_MEMBER_FUNCTION(FreeRoamEntityCameraController::OnMouseScrollCallback);
	Game::Instance().GetWindow()->WindowResizeEventHandler += EVENT_BIND_MEMBER_FUNCTION(FreeRoamEntityCameraController::OnWindowResizeCallback);

	// Camera dragging
	Game::Instance().GetWindow()->MouseButtonPressedEventHandler += EVENT_BIND_MEMBER_FUNCTION(FreeRoamEntityCameraController::OnMousePressedCallback);
	Game::Instance().GetWindow()->MouseButtonReleasedEventHandler += EVENT_BIND_MEMBER_FUNCTION(FreeRoamEntityCameraController::OnMouseReleasedCallback);
	Game::Instance().GetWindow()->MouseMoveEventHandler += EVENT_BIND_MEMBER_FUNCTION(FreeRoamEntityCameraController::OnMouseMoveCallback);
}

// Will fix dragging and moving entity later. Good enough for now.
void FreeRoamEntityCameraController::OnUpdate(Timestep ts)
{
	glm::vec2 move{ 0.0f };

	if (Input::IsKeyPressed(Input::KEY_UP))
		move.y += zoomLevel * ts;
	if (Input::IsKeyPressed(Input::KEY_DOWN))
		move.y -= zoomLevel * ts;
	if (Input::IsKeyPressed(Input::KEY_RIGHT))
		move.x += zoomLevel * ts;
	if (Input::IsKeyPressed(Input::KEY_LEFT))
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

	auto [entityTransform, entityMove] = registry->get<TransformComponent, MoveComponent>(entity);

	if (entityMove.isMoving())
	{
		this->SetPosition(entityTransform.position);
	}
}


void FreeRoamEntityCameraController::OnMousePressedCallback(MouseButtonPressedEventArg& e)
{
	if (e.Button == Input::MOUSE_BUTTON_LEFT)
	{
		mousePressedPoint = Input::GetMousePosition();
		initialCameraPos = GetPosition();
		mouseHeld = true;

		LOG_CORE_INFO("Intial camera pos x:{}, y:{}", initialCameraPos.x, initialCameraPos.y);
		LOG_CORE_INFO("Intial mousepress pos x:{}, y:{}", mousePressedPoint.x, mousePressedPoint.y);
		LOG_CORE_INFO("Bounds width:{}, height:{}", bounds.GetWidth(), bounds.GetHeight());

	}
}

void FreeRoamEntityCameraController::OnMouseReleasedCallback(MouseButtonReleasedEventArg& e)
{
	mouseHeld = false;
}

void FreeRoamEntityCameraController::OnMouseMoveCallback(MouseMoveEventArg& e)
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

		result.x = -result.x;
		SetPosition(initialCameraPos + result);
	}
}

void FreeRoamEntityCameraController::OnMouseScrollCallback(MouseScrolledEventArg& e)
{
	SetZoomLevel(zoomLevel + -e.YOffset * 0.1f * zoomLevel);
}

void FreeRoamEntityCameraController::OnWindowResizeCallback(WindowResizeEventArg& e)
{
	LOG_CORE_INFO("Resize ({},{})", e.Width, e.Height);
	SetAspectRatio((float)e.Width / (float)e.Height);
}


void FreeRoamEntityCameraController::SetEntity(entt::registry* registry, entt::entity entity)
{
	this->entity = entity;
	this->registry = registry;
}

