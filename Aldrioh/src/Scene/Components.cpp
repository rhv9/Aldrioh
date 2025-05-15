#include "pch.h"
#include "Components.h"

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

void MoveComponent::updateMoveVec(const glm::vec2& newMoveVec)
{
	moveVec = newMoveVec;

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
