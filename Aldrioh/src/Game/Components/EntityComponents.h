#pragma once
#include <Scene/Entity.h>
#include <Game/Level/Item/Item.h>

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
	bool addScoreOnDeath = true;
	
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

struct ItemAnimationControllerComponent
{
	bool speedup = false;

	ItemAnimationControllerComponent() = default;
	ItemAnimationControllerComponent(const ItemAnimationControllerComponent&) = default;
};

struct ModularShipComponent
{
	std::array<std::unique_ptr<BaseStatItem>, 5> bsi;
	uint8_t bsiMax = 5, bsiCount = 0;
	
	std::array<std::unique_ptr<ShipModuleItem>, 3> smi;
	uint8_t smiMax = 3, smiCount = 0;
	
	std::array<std::unique_ptr<UniqueItem>, 4> si;
	uint8_t siMax = 4, siCount = 0;

	ModularShipComponent() = default;
	ModularShipComponent(const ModularShipComponent&) = default;

	bool HasItem(const ItemID itemId);
	Item* GetItem(const ItemID itemId);
	void AddItem(const ItemID itemId);
	StatModifier CalculateTotalStatModifier() const;

};

struct StatComponent
{
	StatModifier baseStat;
	StatModifier precomputedBonusStat;
	StatModifier totalCachedStat;
	bool dirty = true;

	StatComponent() = default;
	StatComponent(const StatComponent&) = default;
};

struct ActionComponent
{
	bool up, down, left, right;
	bool shoot;
	glm::vec2 dir;
	float anglePointingTo = 0.0f;
};