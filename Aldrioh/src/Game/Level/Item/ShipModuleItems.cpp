#include <pch.h>
#include "ShipModuleItems.h"

#include <Game/Components/EntityComponents.h>
#include <Scene/Components.h>
#include <Game/Entity/GameEntities.h>
#include <Math/Math.h>

ParticleTemplate particleTemplate_fireballImpact = []() {
	ParticleTemplate pt;
	pt.beginColour = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);
	pt.endColour = glm::vec4(1.0f, 0.5f, 0.0f, 0.9f);
	pt.beginSize = 0.25f;
	pt.endSize = 0.05f;
	pt.life = 0.2f;
	pt.velocity = { 0.0f, 0.0f };
	pt.velocityVariation = { 5.0f, 5.0f };
	pt.rotationRange = { Math::degreesToRad(-45), Math::degreesToRad(45) };
	pt.count = 3;
	return pt;
	}();

auto OnDestroy_FireballImpact = [](Entity fireball) -> void {
	ParticleTemplate pt = particleTemplate_fireballImpact;
	pt.startPos = fireball.GetTransformComponent().position;
	fireball.getScene()->GetParticleManager().Emit(pt);
	};

void shootFireball(Entity& e, const glm::vec2& origin, const glm::vec2& normalizedDir, float dmg, float speed)
{
	// Create entity
	Entity fireball = e.getScene()->CreateEntity("Fireball");
	fireball.GetComponent<TransformComponent>().UpdateBothPos(origin);
	auto& pmc = fireball.AddComponent<PhysicsMovementComponent>(false);
	pmc.resultantVelocity = normalizedDir * speed;
	pmc.naturalFallOffMultiplier = 0.0f;

	VisualComponent& vc = fireball.AddComponent<VisualComponent>(Sprites::bullet_fire, glm::vec3{ -0.5f, -0.5f, 0.0f });
	vc.rotation = Math::angle(normalizedDir);
	vc.colour.a = 1.0f;

	fireball.AddComponent<TimeLifeComponent>(1.0f);
	fireball.AddComponent<EntityTypeComponent>(EntityTypes::Fireball->entityId);

	glm::vec2 collisionSize{ 0.3f };
	fireball.AddComponent<CollisionComponent>(glm::vec3{ collisionSize / -2.0f, 0.0f }, collisionSize);
	fireball.AddComponent<OnDestroyComponent>(OnDestroy_FireballImpact);
	fireball.AddComponent<DamageComponent>(dmg);
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
		dmg_mult += 0.1f;
		return { "Increase damage by 10%" };
	}
}

void FireBallShipModuleItem::OnUpdate(Timestep ts, Entity e)
{
	auto& inputAction = e.GetComponent<ActionComponent>();
	if (inputAction.shoot)
	{
		if (shootTimer >= shootCooldown)
		{
			shootTimer = 0.0f;
			glm::vec2& playerPos = e.GetTransformComponent().position;

			for (int i = 0; i < projectileCount; ++i)
			{
				glm::vec2 dir = Math::angleToNormalizedVector(inputAction.anglePointingTo + i * (Math::PI / 10.0f));
				shootFireball(e, playerPos, dir, cachedDmg, speed);
			}
			e.getScene()->CreateEntity("Sound").AddComponent<SoundComponent>("player_shoot");
		}
	}
	shootTimer += ts;
}

void FireBallShipModuleItem::RecalculateOnStatChanges(StatModifier& statModifier)
{
	cachedDmg = dmg * (dmg_mult + statModifier.dmg_multiplier + 1.0f);
}
