#include <pch.h>
#include "ShipModuleItems.h"

#include <Game/Components/EntityComponents.h>
#include <Scene/Components.h>
#include <Game/Entity/GameEntities.h>
#include <Math/Math.h>

void shootBall(Entity& e, const glm::vec2& origin, const glm::vec2& normalizedDir, float dmg)
{
	float speed = 20.0f;

	// Create entity
	Entity fireball = e.getScene()->CreateEntity("Fireball");
	fireball.GetComponent<TransformComponent>().UpdateBothPos(origin);
	auto& pmc = fireball.AddComponent<PhysicsMovementComponent>(false);
	pmc.resultantVelocity = normalizedDir * speed;
	VisualComponent& vc = fireball.AddComponent<VisualComponent>(Sprites::bullet_fire, glm::vec3{ -0.5f, -0.5f, 0.0f });
	vc.rotation = Math::angle(normalizedDir);
	vc.colour.a = 1.0f;
	fireball.AddComponent<TimeLifeComponent>(1.0f);
	fireball.AddComponent<EntityTypeComponent>(EntityTypes::Fireball->entityId);
	glm::vec2 collisionSize{ 0.3f };
	fireball.AddComponent<CollisionComponent>(glm::vec3{ collisionSize / -2.0f, 0.0f }, collisionSize);
}

LvlUpInfo FireBallShipModuleItem::LevelUp()
{
	++lvl;
	if (lvl % 5 == 0)
	{
		projectileCount += 1;
		return { "Increase projectile count by one" };
	}
	else
	{
		dmg_mult += 10;
		return { "Increase damage by 10%" };
	}
}

void FireBallShipModuleItem::OnUpdate(Timestep ts, Entity e)
{
	auto& inputAction = e.GetComponent<ActionComponent>();
	if (inputAction.shoot)
	{
		if (shootTimer >= 0.25f)
		{
			shootTimer = 0.0f;
			glm::vec2& playerPos = e.GetTransformComponent().position;

			for (int i = 0; i < projectileCount; ++i)
			{
				glm::vec2 dir = Math::angleToNormalizedVector(inputAction.anglePointingTo + i * (Math::PI / 10.0f));
				shootBall(e, playerPos, dir, cachedDmg);
			}
			e.getScene()->CreateEntity("Sound").AddComponent<SoundComponent>("player_shoot");
		}
	}
	shootTimer += ts;
}

void FireBallShipModuleItem::RecalculateOnStatChanges(StatModifier& statModifier)
{
	cachedDmg = (float)dmg * ((dmg_mult + statModifier.dmg_multiplier + 100) / 100.0f);
}
