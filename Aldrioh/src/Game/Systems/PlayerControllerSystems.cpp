#include <pch.h>
#include "PlayerControllerSystems.h"
#include <Systems/HeadersUpdateSystems.h>
#include <Input/Input.h>
#include <Game/Components/ControllerComponents.h>
#include <Game/SpriteCollection.h>

#include <Collision/Collision.h>
#include <Game/Entity/EntityType.h>
#include <Game/Components/EntityComponents.h>

#include <Math/Math.h>

float shootTimer = 0.0f;

void shoot(Entity& e, const glm::vec2& origin, const glm::vec2& normalizedDir)
{
	// Create entity
	Entity fireball = e.getScene()->CreateEntity("Fireball");
	fireball.GetComponent<TransformComponent>().position = glm::vec3{ origin , 0.5f };
	auto& mc = fireball.AddComponent<MoveComponent>(10.0f);
	mc.moveVec = normalizedDir;
	mc.locked = true;
	VisualComponent& vc = fireball.AddComponent<VisualComponent>(Sprites::bullet_fire, glm::vec3{ -0.5f, -0.5f, 0.0f });
	vc.rotation = Math::angle(mc.moveVec);
	vc.colour.a = 1.0f;
	fireball.AddComponent<TimeLifeComponent>(1.0f);
	fireball.AddComponent<EntityTypeComponent>(EntityTypes::Fireball);
	fireball.AddComponent<CollisionBox>(glm::vec3{ -0.5f, -0.5f, 0.0f }, glm::vec2{ 1.0f, 1.0f });
}


glm::vec2 RotatePosition(const glm::vec2& start, const glm::vec2& dest, float x)
{
	float newX = start.x + (dest.x - start.x) * Math::cosRad(x) - (dest.y - start.y) * Math::sinRad(x);
	float newY = start.y + (dest.x - start.x) * Math::sinRad(x) + (dest.y - start.y) * Math::cosRad(x);

	return { newX, newY };
}



void EntitySystem::PlayerControllerSystem(Timestep ts, Scene& scene)
{
	auto view = scene.getRegistry().view<PlayerControllerComponent>();

	for (entt::entity e : view)
	{
		Entity player = scene.WrapEntityHandle(e);

		// Keyboard movement of player
		auto& playerTransform = player.GetComponent<TransformComponent>();
		auto& playerMove = player.GetComponent<MoveComponent>();

		glm::vec2 move{ 0.0f };

		if (Input::IsKeyPressed(Input::KEY_W))
			move.y = 1.0f;
		if (Input::IsKeyPressed(Input::KEY_S))
			move.y = -1.0f;
		if (Input::IsKeyPressed(Input::KEY_A))
			move.x = -1.0f;
		if (Input::IsKeyPressed(Input::KEY_D))
			move.x = 1.0f;

		playerMove.updateMoveVec(move);
		

		PlayerControllerComponent& pcc = view.get<PlayerControllerComponent>(e);

		if (pcc.dirLock == DIRLOCK_FREE)
			player.GetComponent<VisualComponent>().rotation = Math::angleBetween2d(playerTransform.position, { player.getScene()->GetMousePosInScene(), 0.0f });
		else
			player.GetComponent<VisualComponent>().rotation = Math::angle(pcc.dirLock);

		if (Input::IsKeyPressed(Input::KEY_SPACEBAR))
		{
			if (shootTimer >= 0.08f || shootTimer == 0.0f)
			{
				shootTimer = std::max(shootTimer - 1.0f, 0.0f);


				glm::vec3& playerPos = player.GetComponent<TransformComponent>().position;

				glm::vec2 dir;
				if (pcc.dirLock == DIRLOCK_FREE)
				{
					dir = Math::normalizedDirection(glm::vec2(playerPos), player.getScene()->GetMousePosInScene());
				}
				else
					dir = pcc.dirLock;
				shoot(player, playerPos, dir);
				scene.CreateEntity("Sound").AddComponent<SoundComponent>("player_shoot");
				
			}
			shootTimer += ts;
		}
		else
			shootTimer = 0.0f;

		if (Input::IsKeyPressed(Input::KEY_SPACEBAR))
		{

		}
	}
}
