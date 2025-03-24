#include "pch.h"
#include "CollisionManager.h"


// Collision algorithms

static bool collides(CollisionBox* box1, CollisionBox* box2)
{
	const glm::vec2 p1 = box1->position;
	const glm::vec2 p2 = box2->position;

	const glm::vec2 s1 = box1->size;
	const glm::vec2 s2 = box2->size;

	return p1.x + s1.x >= p2.x &&
		p1.x <= p2.x + s2.x &&
		p1.y + s1.y >= p2.y &&
		p1.y <= p2.y + s2.y;
}

static bool collides(CollisionBox* box, CollisionCircle* circle)
{
	return false;
}

static bool collides(CollisionCircle* circle1, CollisionCircle* circle2)
{
	return false;
}


// Box Collision

bool CollisionManagerBox::CollidesWith(CollisionShape* shape, CollisionBox* box)
{
	return collides((CollisionBox*)shape, box);
}

bool CollisionManagerBox::CollidesWith(CollisionShape* shape, CollisionCircle* circle)
{
	return collides((CollisionBox*)shape, circle);
}


// Circle Collision

bool CollisionManagerCircle::CollidesWith(CollisionShape* shape, CollisionBox* box)
{
	return collides(box, (CollisionCircle*)shape);
}

bool CollisionManagerCircle::CollidesWith(CollisionShape* shape, CollisionCircle* circle)
{
	return collides(circle, (CollisionCircle*)shape);
}
