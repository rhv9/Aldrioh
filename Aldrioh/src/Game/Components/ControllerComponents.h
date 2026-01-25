#pragma once

using DIR = glm::vec2;

constexpr DIR DIRLOCK_FREE{ 0 };
constexpr DIR DIRLOCK_UP { 0, 1 };
constexpr DIR DIRLOCK_DOWN { 0, -1 };
constexpr DIR DIRLOCK_LEFT { -1, 0 };
constexpr DIR DIRLOCK_RIGHT { 1, 0 };

struct PlayerControllerComponent
{
	DIR dirLock = DIRLOCK_FREE;
	float radius = 2.5f;
	
	PlayerControllerComponent() = default;
	PlayerControllerComponent(const PlayerControllerComponent&) = default;
};