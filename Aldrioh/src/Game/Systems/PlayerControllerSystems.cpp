#include <pch.h>
#include "PlayerControllerSystems.h"
#include "HeadersUpdateSystems.h"
#include <Input/Input.h>
#include <Game/SpriteCollection.h>

#include <Components/Collision.h>
#include <Game/Entity/EntityType.h>

float shootTimer = 0.0f;

void shoot(Entity& e, const glm::vec2& origin, const glm::vec2& dir)
{
	// Center the direction and origin 
	glm::vec2 dirOffseted = dir;
	glm::vec2 originOffseted = origin;

	// Create entity
	Entity fireball = e.getScene()->CreateEntity("Fireball");
	fireball.GetComponent<TransformComponent>().position = glm::vec3{ originOffseted , 0.5f };
	auto& mc = fireball.AddComponent<MoveComponent>(10.0f);
	mc.moveVec = glm::normalize(dirOffseted - originOffseted);
	mc.locked = true;
	fireball.AddComponent<VisualComponent>(Sprites::fire, glm::vec3{-0.5f, -0.5f, 0.0f});
	fireball.AddComponent<TimeLifeComponent>(1.0f);
	fireball.AddComponent<EntityTypeComponent>(EntityType::Fireball);
	fireball.AddComponent<CollisionBox>(glm::vec3{ -0.5f, -0.5f, 0.0f }, glm::vec2{ 1.0f, 1.0f });
}



void EntitySystem::PlayerControllerSystem(Timestep ts, Scene& scene)
{
	Entity& player = *scene.GetPlayer();

	// Keyboard movement of player
	{
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
	}

	if (Input::IsKeyPressed(Input::KEY_F))
	{
		if (shootTimer >= 0.1f || shootTimer == 0.0f)
		{
			shootTimer = std::max(shootTimer - 1.0f, 0.0f);
			LOG_TRACE("Shooting!");
			glm::vec3& playerPos = player.GetComponent<TransformComponent>().position;

			shoot(player, playerPos, player.getScene()->GetMousePosInScene());
		}
		shootTimer += ts;
	}
	else
		shootTimer = 0.0f;


	if (Input::IsKeyPressed(Input::KEY_SPACEBAR))
	{
		auto& jc = player.GetComponent<JumpComponent>();

		if (jc.z == 0)
		{
			//jc.acceleration = 0.05f;
			jc.velocity = 0.14f;
		}
	}


}
