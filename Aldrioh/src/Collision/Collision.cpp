#include "pch.h"

#include "Collision.h"
#include "CollisionManager.h"


CollisionManager* CollisionBox::collisionManager = new CollisionManagerBox();
CollisionManager* CollisionCircle::collisionManager = new CollisionManagerCircle();


bool CollisionBox::CollidesWith(CollisionShape* shape)
{
    return shape->GetCollisionManager()->CollidesWith(shape, this);
}

CollisionManager* CollisionBox::GetCollisionManager()
{
    return CollisionBox::collisionManager;
}

bool CollisionCircle::CollidesWith(CollisionShape* shape)
{
    return shape->GetCollisionManager()->CollidesWith(shape, this);
}

CollisionManager* CollisionCircle::GetCollisionManager()
{
    return CollisionCircle::collisionManager;
}
