#pragma once
#include <Game/Entity/EntityType.h>
#include <Scene/CameraController.h>
#include <Scene/Entity.h>

class UIManager;
struct UIManagerComponent
{
	std::unique_ptr<UIManager> uiManager = nullptr;

	UIManagerComponent() = default;
	UIManagerComponent(const UIManagerComponent&) = default;
};

struct EntityTypeComponent
{
	EntityType type;

	EntityTypeComponent() = default;
	EntityTypeComponent(const EntityTypeComponent&) = default;
	EntityTypeComponent(EntityType type) : type(type) {}
};

struct TransformComponent
{
	glm::vec3 position { 0 };
	
	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
	TransformComponent(const glm::vec3& pos) : position(pos) {}
};

struct VisualComponent
{
	int spriteId;
	glm::vec3 localTransform;
	glm::vec2 scale;
	float rotation = 0.0f;
	glm::vec4 colour{ 1.0f };
	float flags = 0;

	VisualComponent(const int spriteId, const glm::vec3& localTransform, const glm::vec2& scale) : spriteId(spriteId), localTransform(localTransform), scale(scale) {}
	VisualComponent(const int spriteId, const glm::vec3& localTransform) : VisualComponent(spriteId, localTransform, DEFAULT_SCALE) {}
	VisualComponent(const int spriteId) : VisualComponent(spriteId, DEFAULT_LOCAL_TRANSFORM, DEFAULT_SCALE) {}
	VisualComponent() : VisualComponent(DEFAULT_SPRITE_ID, DEFAULT_LOCAL_TRANSFORM, DEFAULT_SCALE) {}

	VisualComponent(const VisualComponent&) = default;

	static constexpr int DEFAULT_SPRITE_ID = 0;
	static constexpr glm::vec3 DEFAULT_LOCAL_TRANSFORM { 0.0f, 0.0f, 0.0f };
	static constexpr glm::vec2 DEFAULT_SCALE { 1.0f, 1.0f };
};

struct DestroyEntityComponent
{
	float timeRemaining = 0.0f;

	DestroyEntityComponent() = default;
	DestroyEntityComponent(float timer) :  timeRemaining(timer) {}
	DestroyEntityComponent(DestroyEntityComponent&) = default;
};

struct OnDestroyComponent
{
	std::function<void(Entity)> onDeathFunc;

	OnDestroyComponent() = default;
	OnDestroyComponent(std::function<void(Entity)> onDeathFunc) : onDeathFunc(onDeathFunc) {}
	OnDestroyComponent(OnDestroyComponent&) = default;
};

enum MoveDir
{
	UP = 0,
	DOWN,
	LEFT,
	RIGHT,
	NONE
};

struct MoveComponent
{
	float speed;
	glm::vec2 moveVec{ 0.0f };
	MoveDir dir = MoveDir::NONE;
	bool locked = false;

	MoveComponent(float speed) : speed(speed) {}
	MoveComponent() : speed(16.0f) {}
	MoveComponent(const MoveComponent&) = default;

	bool isMoving() const { return moveVec != ZERO_VEC; }
	bool isMovingUp() const { return moveVec.y > 0; }
	bool isMovingDown() const { return moveVec.y < 0; }
	bool isMovingLeft() const { return moveVec.x < 0; }
	bool isMovingRight() const { return moveVec.x > 0; }
	MoveDir getMoveDir() const { return dir; }

	void updateMoveVec(const glm::vec2& newMoveVec);

	static constexpr glm::vec2 ZERO_VEC{ 0.0f };
	void zero() { moveVec = ZERO_VEC; }
};


struct AnimateVisualComponent
{
	std::vector<int> spriteIds;

	float ts = 0.0f;
	float speed;
	int frame = 0;

	AnimateVisualComponent(const std::vector<int> spriteIds, float speed) : spriteIds(spriteIds), speed(speed) {}
	AnimateVisualComponent() : AnimateVisualComponent({ 0 }, 1.0f) {}
	AnimateVisualComponent(const AnimateVisualComponent&) = default;

	int getCurrentSprite() { return spriteIds[frame]; }
	void reset() { ts = 0.0f; }
};

struct AnimatedMovementComponent
{
	std::vector<std::vector<int>> animations;
	float ts = 0.0f;
	float speed = 1.0f;
	int frame = 0;
	MoveDir currentDir = MoveDir::UP;
	
	AnimatedMovementComponent(std::vector<int>& up, std::vector<int>& down, std::vector<int>& left, std::vector<int>& right, float speed) : animations({up, down, left, right}), speed(speed) {}
	AnimatedMovementComponent() = default;
	AnimatedMovementComponent(const AnimatedMovementComponent&) = default;

	void update(float delta);
	int getCurrentSprite() { return animations[static_cast<int>(currentDir)][frame]; }
	void reset() { ts = speed; frame = 0; }
};

struct RandomMovementComponent
{
	float speed;

	RandomMovementComponent() : speed(16 * 2) {}
	RandomMovementComponent(float speed) : speed(speed) {}
	RandomMovementComponent(const RandomMovementComponent&) = default;
};

struct CameraComponent
{
	CameraController* cameraController;

	CameraComponent();
	CameraComponent(CameraController* cameraController) : cameraController(cameraController) {}
	CameraComponent(const CameraComponent&) = default;
};



struct NameComponent
{
	static int entityCount;

	std::string name;

	NameComponent() = default;
	NameComponent(const NameComponent&) = default;
	NameComponent(const std::string& name) : name(name + std::to_string(++entityCount)) {}
};

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
	float move = 1;
	float speed = 1;
	float startTime = -1;
	float startX = 0;
	float distance = 1;
	float elapsedTime = 0;
	float alreadyMoved = 0;

	float entityCount = 0;

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

struct JumpComponent
{
	float z;
	float velocity;
	float acceleration;

	JumpComponent() = default;
	JumpComponent(const JumpComponent&) = default;
};









