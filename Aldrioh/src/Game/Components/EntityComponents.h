#pragma once
#include <Scene/Entity.h>

struct TimeLifeComponent
{
	float timeRemaining = 0.0f;

	TimeLifeComponent() = default;
	TimeLifeComponent(const TimeLifeComponent&) = default;
	TimeLifeComponent(const float lifeSpan) : timeRemaining(lifeSpan) {}
};

struct HealthComponent
{
	float maxHealth = 1;
	float health = 1;

	HealthComponent() = default;
	HealthComponent(const HealthComponent&) = default;
	HealthComponent(float maxHealth) : maxHealth(maxHealth), health(maxHealth) {}
};

struct EnemyManagerComponent
{
	float entityCount = 0;

	std::function<void(Timestep, Entity)> OnUpdateFunc;

	EnemyManagerComponent() = default;
	EnemyManagerComponent(const EnemyManagerComponent&) = default;
};

struct GlobalDumbAIComponent
{
	Entity enemyManager;
	float distance = 1;
	float move = 1;

	bool firstUpdate = true;

	GlobalDumbAIComponent() = default;
	GlobalDumbAIComponent(const GlobalDumbAIComponent&) = default;
};

struct FollowPlayerAIComponent
{
	bool follow = true;

	FollowPlayerAIComponent() = default;
	FollowPlayerAIComponent(const FollowPlayerAIComponent&) = default;
};

enum class HitVisualState : uint8_t
{
	NORMAL = 0,
	JUST_HIT,
	COUNTING_DOWN,
	FINISH
};

struct CoreEnemyStateComponent
{
	float hitVisualTimer = 0;
	HitVisualState hitVisualState = HitVisualState::NORMAL;
	glm::vec4 hitvisualOriginalColour{ 0.0f };
	
	CoreEnemyStateComponent() = default;
	CoreEnemyStateComponent(const CoreEnemyStateComponent&) = default;
};

struct JumpComponent
{
	float z;
	float velocity;
	float acceleration;

	JumpComponent() = default;
	JumpComponent(const JumpComponent&) = default;
};


struct ShipComponent
{
	glm::vec2 directionFacing;

	ShipComponent() = default;
	ShipComponent(const ShipComponent&) = default;
};

struct RandomMovementComponent
{
	float speed;

	RandomMovementComponent() : speed(16 * 2) {}
	RandomMovementComponent(float speed) : speed(speed) {}
	RandomMovementComponent(const RandomMovementComponent&) = default;
};

