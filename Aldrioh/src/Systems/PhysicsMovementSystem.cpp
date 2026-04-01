#include <pch.h>
#include "HeadersUpdateSystems.h"
#include "PhysicsMovementSystem.h"
#include <Math/Math.h>

void EntitySystem::PhysicsMovementSystem(Timestep ts, Scene& scene)
{
	static std::vector<Entity> collidedEntities;
	{
		auto view = scene.getRegistry().view<MoveControllerComponent, PhysicsMovementComponent>();

		for (entt::entity eHandle : view)
		{
			auto [mcc, pmc] = view.get<MoveControllerComponent, PhysicsMovementComponent>(eHandle);
			pmc.resultantVelocity += mcc.moveDir * mcc.speed;
		}
	}


	{
		auto view = scene.getRegistry().view<TransformComponent, PhysicsMovementComponent>();

		for (entt::entity eHandle : view)
		{
			auto [t1, pmc1] = view.get<TransformComponent, PhysicsMovementComponent>(eHandle);
			pmc1.resultantVelocity *= 0.95f;
			t1.position += pmc1.resultantVelocity * (float)ts;
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
							PhysicsMovementComponent pmc2 = e2.GetComponent<PhysicsMovementComponent>();

							float pushout = 0.32f;
							glm::vec2 direction = cb1Offseted.GetMidpoint() - cb2Offseted.GetMidpoint();
							glm::vec2 normalizedDirection = glm::normalize(direction);

							pmc1.resultantVelocity += glm::vec2{ normalizedDirection.x ,normalizedDirection.y } * pushout;
							pmc2.resultantVelocity += glm::vec2{ normalizedDirection.x, normalizedDirection.y }* -pushout;
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
	}

}
