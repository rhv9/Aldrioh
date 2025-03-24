#pragma once

#include "Collision.h"

class CollisionManager
{
public:

	virtual bool CollidesWith(CollisionShape* shape, CollisionBox* box) = 0;
	virtual bool CollidesWith(CollisionShape* shape, CollisionCircle* circle) = 0;
};

class CollisionManagerBox : public CollisionManager
{
public:

	virtual bool CollidesWith(CollisionShape* shape, CollisionBox* box) override;
	virtual bool CollidesWith(CollisionShape* shape, CollisionCircle* circle) override;
};

class CollisionManagerCircle : public CollisionManager
{
public:

	virtual bool CollidesWith(CollisionShape* shape, CollisionBox* box) override;
	virtual bool CollidesWith(CollisionShape* shape, CollisionCircle* circle) override;
};