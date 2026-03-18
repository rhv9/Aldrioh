#include <pch.h>
#include "ShipModuleItems.h"

#include <Game/Components/EntityComponents.h>
#include <Scene/Components.h>
#include <Game/Entity/GameEntities.h>
#include <Math/Math.h>

void shootBall(Entity& e, const glm::vec2& origin, const glm::vec2& normalizedDir)
{
	// Create entity
	Entity fireball = e.getScene()->CreateEntity("Fireball");
	fireball.GetComponent<TransformComponent>().position = glm::vec3{ origin , 0.5f };
	auto& mc = fireball.AddComponent<MoveComponent>(20.0f);
	mc.addMoveVec(normalizedDir);
	mc.locked = true;
	VisualComponent& vc = fireball.AddComponent<VisualComponent>(Sprites::bullet_fire, glm::vec3{ -0.5f, -0.5f, 0.0f });
	vc.rotation = Math::angle(mc.moveVec);
	vc.colour.a = 1.0f;
	fireball.AddComponent<TimeLifeComponent>(1.0f);
	fireball.AddComponent<EntityTypeComponent>(EntityTypes::Fireball->entityId);
	glm::vec2 collisionSize{ 0.3f };
	fireball.AddComponent<CollisionComponent>(glm::vec3{ collisionSize / -2.0f, 0.0f }, collisionSize);
}

LvlUpInfo FireBallShipModuleItem::LevelUp()
{

}

void FireBallShipModuleItem::OnUpdate(Timestep ts, Entity e)
{
	auto& inputAction = e.GetComponent<ActionComponent>();
	if (inputAction.shoot)
	{
		if (shootTimer >= 0.08f || shootTimer == 0.0f)
		{
			shootTimer = std::max(shootTimer - 1.0f, 0.0f);
			glm::vec3& playerPos = e.GetTransformComponent().position;
			shootBall(e, playerPos, inputAction.dir);
			e.getScene()->CreateEntity("Sound").AddComponent<SoundComponent>("player_shoot");

		}
		shootTimer += ts;
	}
	else
		shootTimer = 0.0f;
}
