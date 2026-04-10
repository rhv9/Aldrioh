#pragma once
#include <Game/Entity/EntityType.h>
#include <Scene/CameraController.h>
#include <Scene/Entity.h>
#include <Graphics/RenderQueue.h>
#include <Collision/Collision.h>

class UIManager;
struct UIManagerComponent
{
	std::unique_ptr<UIManager> uiManager = nullptr;

	UIManagerComponent() = default;
	UIManagerComponent(const UIManagerComponent&) = default;
};

struct EntityTypeComponent
{
	EntityID typeId;

	EntityTypeComponent() = default;
	EntityTypeComponent(const EntityTypeComponent&) = default;
	EntityTypeComponent(EntityID entityId) : typeId(entityId) {}
};

struct TransformComponent
{
	glm::vec2 position{ 0.0f };
	glm::vec2 prevPosition{ 0.0f };

	glm::vec2 CalculateInterpolatePosition(float deltaPercent);

	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
	TransformComponent(const glm::vec2& pos) : position(pos), prevPosition(pos) {}

	void UpdateBothPos(const glm::vec2& pos) { position = prevPosition = pos; }
};

struct VisualComponent
{
	spriteid_t spriteId;
	glm::vec3 localTransform;
	glm::vec2 scale;
	float rotation = 0.0f;
	glm::vec4 colour{ 1.0f };
	float flags = 0;
	RenderLayer renderLayer = RenderLayer::TWO;

	VisualComponent(const spriteid_t spriteId, const glm::vec3& localTransform, const glm::vec2& scale) : spriteId(spriteId), localTransform(localTransform), scale(scale) {}
	VisualComponent(const spriteid_t spriteId, const glm::vec3& localTransform) : VisualComponent(spriteId, localTransform, DEFAULT_SCALE) {}
	VisualComponent(const spriteid_t spriteId) : VisualComponent(spriteId, DEFAULT_LOCAL_TRANSFORM, DEFAULT_SCALE) {}
	VisualComponent() : VisualComponent(DEFAULT_SPRITE_ID, DEFAULT_LOCAL_TRANSFORM, DEFAULT_SCALE) {}

	VisualComponent(const VisualComponent&) = default;

	static constexpr int DEFAULT_SPRITE_ID = 0;
	static constexpr glm::vec3 DEFAULT_LOCAL_TRANSFORM{ 0.0f, 0.0f, 0.0f };
	static constexpr glm::vec2 DEFAULT_SCALE{ 1.0f, 1.0f };
};

struct DestroyEntityComponent
{
	float timeRemaining = 0.0f;

	DestroyEntityComponent() = default;
	DestroyEntityComponent(float timer) : timeRemaining(timer) {}
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


struct PhysicsMovementComponent
{
	glm::vec2 resultantVelocity{ 0.0f, 0.0f };
	glm::vec2 managedVelocity{ 0.0f };
	float naturalFallOffPercent = 0.95f;

	PhysicsMovementComponent() = default;
	PhysicsMovementComponent(bool naturalSlowdown) : naturalFallOffPercent(naturalSlowdown ? 0.95f : -1.0f) {}
	PhysicsMovementComponent(const PhysicsMovementComponent&) = default;
};

struct MoveControllerComponent
{
	glm::vec2 moveDir{ 0.0f, 0.0f };
	float speed = 1.0f;
	float maxSpeed = 1.0f;
	glm::vec2 velocity;
	float falloff = 0.5f;

	bool IsMoving() const { return moveDir != glm::vec2(0.0f); }

	MoveControllerComponent() = default;
	MoveControllerComponent(float speed) : speed(speed), moveDir(0.0f) {}
	MoveControllerComponent(const MoveControllerComponent&) = default;
};

struct BezierPathComponent
{
	float t = 0;
	glm::vec2 p0{ 0 }, p1{ 0 }, p2{ 0 };
	std::function<void(Entity e)> onCompletionCallback;
	bool completionHandled = false;

	BezierPathComponent(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2) : p0(p0), p1(p1), p2(p2) {}
	BezierPathComponent(const BezierPathComponent&) = default;
};

struct RotationComponent
{
	float angle = 0.0f;
	bool shouldRotateTo = false;
	uint8_t skipTicks = 1;

	RotationComponent() = default;
	RotationComponent(float angle) : angle(angle) {};
	RotationComponent(const RotationComponent&) = default;
};


struct AnimateVisualComponent
{
	std::vector<spriteid_t> spriteIds;

	float ts = 0.0f;
	float speed;
	int frame = 0;

	AnimateVisualComponent(const std::vector<spriteid_t> spriteIds, float speed) : spriteIds(spriteIds), speed(speed) {}
	AnimateVisualComponent() : AnimateVisualComponent({ 0 }, 1.0f) {}
	AnimateVisualComponent(const AnimateVisualComponent&) = default;

	spriteid_t getCurrentSprite() { return spriteIds[frame]; }
	void reset() { ts = 0.0f; }
};

struct AnimatedMovementComponent
{
	std::vector<std::vector<int>> animations;
	float ts = 0.0f;
	float speed = 1.0f;
	int frame = 0;
	MoveDir currentDir = MoveDir::UP;

	AnimatedMovementComponent(std::vector<int>& up, std::vector<int>& down, std::vector<int>& left, std::vector<int>& right, float speed) : animations({ up, down, left, right }), speed(speed) {}
	AnimatedMovementComponent() = default;
	AnimatedMovementComponent(const AnimatedMovementComponent&) = default;

	void update(float delta);
	int getCurrentSprite() { return animations[static_cast<int>(currentDir)][frame]; }
	void reset() { ts = speed; frame = 0; }
};

struct CameraComponent
{
	std::unique_ptr<CameraController> cameraController;
	bool primary = false;

	CameraComponent();
	CameraComponent(std::unique_ptr<CameraController> cameraController) : cameraController(std::move(cameraController)) {}
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

struct SoundComponent
{
	std::string soundName;

	SoundComponent(const std::string& soundName) : soundName(soundName) {}
	SoundComponent(const SoundComponent&) = default;
};

// CollisionDispatcher adds this component to make it easy to test whether its collision has already been dealt with and not to handle any other collision with that entity. Easy to test if an entity has this and iterate to remove this component.
struct CollisionHandledComponent
{
	bool handled = true;

	CollisionHandledComponent() = default;
	CollisionHandledComponent(bool handled) : handled(handled) {}
	CollisionHandledComponent(const CollisionHandledComponent&) = default;
};


struct CollisionComponent
{
	CollisionBox collisionBox;
	bool rigid = false;

	CollisionComponent(const CollisionBox& collisionBox) : collisionBox(collisionBox) {}
	CollisionComponent(const glm::vec3& pos, const glm::vec2& size, bool rigid = false) : collisionBox(pos, size), rigid(rigid) {}
	CollisionComponent(const CollisionComponent&) = default;
};




