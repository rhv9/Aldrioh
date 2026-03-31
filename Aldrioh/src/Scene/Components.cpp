#include "pch.h"
#include "Components.h"
#include <Game.h>

int NameComponent::entityCount = 0;

void AnimatedMovementComponent::update(float delta)
{
	ts += delta;
	while (ts > speed)
	{
		ts -= speed;
		frame = (frame + 1) % animations[static_cast<int>(currentDir == MoveDir::NONE ? MoveDir::UP : currentDir)].size();
	}
}

glm::vec2 MoveComponent::CalculateActualMoveOffsetVec2(Timestep ts)
{
	return moveVec * (float)ts;
}

void MoveComponent::addMoveVec(const glm::vec2& newMoveVec, float speed)
{
	moveVec += newMoveVec * speed;

	if (moveVec.x > 0)
		dir = MoveDir::RIGHT;
	else if (moveVec.x < 0)
		dir = MoveDir::LEFT;
	else if (moveVec.y > 0)
		dir = MoveDir::UP;
	else if (moveVec.y < 0)
		dir = MoveDir::DOWN;
	else
		dir = MoveDir::NONE;
}

CameraComponent::CameraComponent()
{
	float aspectRatio = static_cast<float>(Game::Instance().GetWindow()->GetHeight()) / Game::Instance().GetWindow()->GetWidth();
	cameraController = std::make_unique<CameraController>(aspectRatio, 5.0f);
}

glm::vec2 TransformComponent::CalculateInterpolatePosition(float deltaPercent)
{
	const glm::vec2& diff = position - prevPosition;
	return { position.x - diff.x * (1.0f - deltaPercent), position.y - diff.y * (1.0f - deltaPercent) };
}
