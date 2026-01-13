#pragma once

class CollisionManager;

class CollisionShape 
{
public:
	virtual bool CollidesWith(CollisionShape* shape) = 0;
	virtual CollisionManager* GetCollisionManager() = 0;
};

class CollisionBox : public CollisionShape 
{
public:
	glm::vec3 position;
	glm::vec2 size;

	static CollisionManager* collisionManager;

	CollisionBox(const glm::vec3& pos, const glm::vec2& size)
		: position(pos), size(size) {}

	CollisionBox OffsetNew(const glm::vec3& offset) { return CollisionBox(position + offset, size); }
	glm::vec2 GetMidpoint() const { return { position.x + size.x / 2.0f, position.y + size.y / 2.0f }; }

	virtual bool CollidesWith(CollisionShape* shape) override;
	virtual CollisionManager* GetCollisionManager() override;
};

class CollisionCircle : public CollisionShape
{
public:
	glm::vec2 position;
	float radius;

	static CollisionManager* collisionManager;

	CollisionCircle(const glm::vec2& pos, float radius)
		: position(pos), radius(radius) {}

	virtual bool CollidesWith(CollisionShape* shape) override;
	virtual CollisionManager* GetCollisionManager() override;
};