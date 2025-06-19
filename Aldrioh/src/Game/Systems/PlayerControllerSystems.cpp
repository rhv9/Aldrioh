#include <pch.h>
#include "PlayerControllerSystems.h"
#include "HeadersUpdateSystems.h"
#include <Input/Input.h>
#include <Game/SpriteCollection.h>

float shootTimer = 0.0f;

void shoot(Entity& e, const glm::vec2& origin, const glm::vec2& direction)
{
	// Create entity
	Entity fireball = e.getScene()->CreateEntity("Fireball");
	fireball.GetComponent<TransformComponent>().position = glm::vec3{ origin , 0.5f };
	fireball.AddComponent<MoveComponent>(10.0f).moveVec = glm::normalize(direction - origin);
	fireball.AddComponent<VisualComponent>(Sprites::fire);
	fireball.AddComponent<TimeLifeComponent>(1.0f);
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
		if (shootTimer >= 1.0f || shootTimer == 0.0f)
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


	if (Input::IsKeyPressed(Input::KEY_SPACE))
	{
		auto& jc = player.GetComponent<JumpComponent>();

		if (jc.z == 0)
		{
			//jc.acceleration = 0.05f;
			jc.velocity = 0.14f;
		}
	}
}
