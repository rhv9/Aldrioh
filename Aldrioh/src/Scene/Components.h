#pragma once


struct TransformComponent
{
	glm::vec3 position;
	
	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
	TransformComponent(const glm::vec3& pos) : position(pos) {}
};

struct VisualComponent
{
	int spriteId;
	glm::vec3 localTransform;
	glm::vec2 scale;

	VisualComponent(const int spriteId, const glm::vec3& localTransform, const glm::vec2& scale) : spriteId(spriteId), localTransform(localTransform), scale(scale) {}
	VisualComponent(const int spriteId, const glm::vec3& localTransform) : VisualComponent(spriteId, localTransform, DEFAULT_SCALE) {}
	VisualComponent(const int spriteId) : VisualComponent(spriteId, DEFAULT_LOCAL_TRANSFORM, DEFAULT_SCALE) {}
	VisualComponent() : VisualComponent(DEFAULT_SPRITE_ID, DEFAULT_LOCAL_TRANSFORM, DEFAULT_SCALE) {}

	VisualComponent(const VisualComponent&) = default;

	static constexpr int DEFAULT_SPRITE_ID = 0;
	static constexpr glm::vec3 DEFAULT_LOCAL_TRANSFORM { 0.0f, 0.0f, 0.0f };
	static constexpr glm::vec2 DEFAULT_SCALE { 1.0f, 1.0f };
};

struct RandomMovementComponent
{
	float speed;

	RandomMovementComponent(float speed) : speed(speed) {}
	RandomMovementComponent() : speed(16 * 2) {}
};

struct MoveComponent
{
	float speed;
	glm::vec2 moveVec{ 0.0f };

	MoveComponent(float speed) : speed(speed) {}
	MoveComponent() : speed(16.0f) {}
	MoveComponent(const MoveComponent&) = default;

	bool isMoving() { return moveVec != ZERO_VEC; }

	static constexpr glm::vec2 ZERO_VEC{ 0.0f };
	void zero() { moveVec = ZERO_VEC; }
};

struct NameComponent
{
	static int entityCount;

	std::string name;

	NameComponent() = default;
	NameComponent(const NameComponent&) = default;
	NameComponent(const std::string& name) : name(name + std::to_string(++entityCount)) {}
};





