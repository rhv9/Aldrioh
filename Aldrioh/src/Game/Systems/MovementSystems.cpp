#include "pch.h"
#include "MovementSystems.h"
#include <Game/Components/EntityComponents.h>
#include <Systems/HeadersUpdateSystems.h>

#include <Collision/CollisionDispatcher.h>
#include <Collision/Collision.h>

void EntitySystem::ResetMovementSystem(Timestep ts, Scene& scene)
{
	// Reset Move Component
	auto view = scene.getRegistry().view<MoveComponent>();

	for (entt::entity e : view)
	{
		auto& mc = view.get<MoveComponent>(e);
		if (!mc.locked) mc.zero();
	}
}

void EntitySystem::MovementSystem(Timestep ts, Scene& scene)
{
	static std::vector<Entity> collidedEntities;
	auto view = scene.getRegistry().view<TransformComponent, MoveComponent>();

	for (entt::entity eHandle : view)
	{
		auto [t1, move1] = view.get(eHandle);
		Entity e1 = scene.WrapEntityHandle(eHandle);
		if (e1.HasComponent<CollisionComponent>())
		{
			bool collided = scene.GetCollisions(ts, e1, collidedEntities);

			if (collided)
			{
				for (auto& e2 : collidedEntities)
				{
					if (e1.HasComponent<CollisionHandledComponent>())
						break;
					if (e2.HasComponent<CollisionHandledComponent>())
						continue;

					//this needs to be moved out boi
					CollisionComponent& cc1 = e1.GetComponent<CollisionComponent>();
					CollisionComponent& cc2 = e2.GetComponent<CollisionComponent>();

					if (cc1.rigid && cc2.rigid)
					{
						CollisionBox cb1Offseted = cc1.collisionBox.OffsetNew(t1.position);

						CollisionBox cb2Offseted = cc2.collisionBox.OffsetNew(e2.GetTransformComponent().position);
						MoveComponent move2 = e2.GetComponent<MoveComponent>();

						float pushout = 0.32f;
						glm::vec2 direction = cb1Offseted.GetMidpoint() - cb2Offseted.GetMidpoint();
						glm::vec2 normalizedDirection = glm::normalize(direction);

						move1.addMoveVec({ normalizedDirection.x ,normalizedDirection.y }, pushout);
						move2.addMoveVec({ normalizedDirection.x, normalizedDirection.y }, -pushout);
					}

					CollisionEvent eventEntity1{ e1, false };
					CollisionEvent eventEntity2{ e2, false };
					scene.GetCollisionDispatcher().Dispatch(eventEntity1, eventEntity2);

					if (eventEntity1.handled)
						e1.AddComponent<CollisionHandledComponent>();
					if (eventEntity2.handled)
						e2.AddComponent<CollisionHandledComponent>();
				}

				collidedEntities.clear();
			}

		}
	}

	for (entt::entity e : view)
	{
		auto [transform, move] = view.get(e);
		transform.position += move.CalculateActualMoveOffsetVec2(ts);
	}

	// Remove all handled collision components
	{
		auto view = scene.getRegistry().view<CollisionHandledComponent>();

		for (auto e : view)
			scene.WrapEntityHandle(e).RemoveComponent<CollisionHandledComponent>();
	}
}

void EntitySystem::LifeSystem(Timestep ts, Scene& scene)
{
	auto view = scene.getRegistry().view<TimeLifeComponent>();

	for (entt::entity e : view)
	{
		auto [tlc] = view.get(e);
		if (tlc.timeRemaining <= 0.0f)
		{
			Entity entity = scene.WrapEntityHandle(e);
			entity.QueueDestroy();
		}
		else
			tlc.timeRemaining -= ts;
	}
}
