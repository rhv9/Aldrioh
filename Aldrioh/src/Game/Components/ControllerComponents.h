#pragma once

constexpr glm::vec2 DIRLOCK_FREE{ 0 };
constexpr glm::vec2 DIRLOCK_UP { 0, 1 };
constexpr glm::vec2 DIRLOCK_DOWN { 0, -1 };
constexpr glm::vec2 DIRLOCK_LEFT { -1, 0 };
constexpr glm::vec2 DIRLOCK_RIGHT { 1, 0 };


struct PlayerControllerComponent
{
	glm::vec2 dirLock = DIRLOCK_FREE;
	
	PlayerControllerComponent() = default;
	PlayerControllerComponent(const PlayerControllerComponent&) = default;
};