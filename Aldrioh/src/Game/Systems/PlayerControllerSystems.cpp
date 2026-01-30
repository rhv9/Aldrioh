#include <pch.h>
#include "PlayerControllerSystems.h"
#include <Systems/HeadersUpdateSystems.h>
#include <Input/Input.h>
#include <Game/Components/ControllerComponents.h>
#include <Game/SpriteCollection.h>

#include <Collision/Collision.h>
#include <Game/Entity/EntityType.h>
#include <Game/Components/EntityComponents.h>
#include <Game/Level/CollectableManager.h>
#include <Game/Components/LevelComponents.h>

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
	glm::vec2 collisionSize{ 0.3f };
	fireball.AddComponent<CollisionComponent>(glm::vec3{ collisionSize / -2.0f, 0.0f }, collisionSize);
}


glm::vec2 RotatePosition(const glm::vec2& start, const glm::vec2& dest, float x)
{
	float newX = start.x + (dest.x - start.x) * Math::cosRad(x) - (dest.y - start.y) * Math::sinRad(x);
	float newY = start.y + (dest.x - start.x) * Math::sinRad(x) + (dest.y - start.y) * Math::cosRad(x);

	return { newX, newY };
}

ParticleTemplate playerExhaustParticle = []() -> ParticleTemplate {
	ParticleTemplate pt;
	pt.beginColour = glm::vec4(1.0f * 0.8f, 0.5f * 0.8f, 0.0f, 1.0f);
	pt.endColour = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	pt.beginSize = 0.2f;
	pt.endSize = 0.2f;
	pt.life = 1.0f;
	pt.velocity = { 0.0f, 0.0f };
	pt.velocityVariation = { 1.4f, 1.4f };
	pt.rotationRange = { Math::degreesToRad(-45), Math::degreesToRad(45) };
	return pt;
	}();


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
		glm::vec2 mousePos = scene.GetMousePosInScene();

		float angleBetweenPlayerAndMouse = Math::angleBetween2d(playerTransform.position, { mousePos, 0.0f });

		if (pcc.dirLock == DIRLOCK_FREE)
			player.GetComponent<VisualComponent>().rotation = angleBetweenPlayerAndMouse;
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


		if (move.x != 0 || move.y != 0)
		{
			glm::vec2 posOffset = glm::normalize(glm::vec2{ playerTransform.position } - mousePos) * 0.6f;

			ParticleTemplate pt = playerExhaustParticle;
			pt.startPos = glm::vec2{ playerTransform.position } + posOffset;
			scene.GetParticleManager().Emit(pt);
		}


		// Simple brute force algorithm to select cells that are within radius from center of cell
		Level* level = scene.GetFirstEntity<LevelComponent>().GetComponent<LevelComponent>().level;
		CollectableManager& collectableManager = level->GetCollectableManager();

		int startX = playerTransform.position.x - pcc.radius;
		int startY = playerTransform.position.y - pcc.radius;
		int endX = playerTransform.position.x + pcc.radius;
		int endY = playerTransform.position.y + pcc.radius;

		for (int y = startY; y < endY; ++y)
		{
			for (int x = startX; x < endX; ++x)
			{
				if (Math::dist(glm::vec2{ (float)x + 0.5f, (float)y + 0.5f }, glm::vec2(playerTransform.position)) <= pcc.radius)
				{
					glm::vec2 chunkPos = { x, y };
					CollectableMapping mapping = collectableManager.GetMapping(chunkPos);
					CollectableCell& cell = collectableManager.GetChunk(mapping).GetCell(mapping);

					for (int i = 0; i < cell.count; ++i)
					{
						CellItem& cellData = cell.cellArray[i];
						CellItem::RenderData itemRenderData = cellData.GetRenderData();
						Entity itemEntity = scene.CreateEntity("Item");
						VisualComponent& vc = itemEntity.AddComponent<VisualComponent>(itemRenderData.spriteId);
						vc.scale = itemRenderData.size;
						vc.localTransform = { -(vc.scale / 2.0f), 0.0f };
						vc.colour = itemRenderData.colour;
						glm::vec2 p0 = cellData.GetFloatOffset() + chunkPos;
						itemEntity.GetTransformComponent().position = { p0, 0.0f };
						BezierPathComponent& bezier = itemEntity.AddComponent<BezierPathComponent>(p0, p0 + glm::vec2{ 0.0f, 1.5f }, playerTransform.position);
						bezier.onCompletionCallback = [](Entity e) {
							e.QueueDestroy();
							LOG_INFO("YIPEEE!");
							};
						itemEntity.AddComponent<ItemAnimationControllerComponent>();
					}
					cell.Clear();
				}
			}
		}
	}
}
