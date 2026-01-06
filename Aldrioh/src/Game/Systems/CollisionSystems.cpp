#include "pch.h"
#include "CollisionSystems.h"
#include <Systems/HeadersUpdateSystems.h>
#include <Collision/Collision.h>

void EntitySystem::CollisionSystem(Timestep ts, Scene& scene)
{
	{
		auto view = scene.getRegistry().view<TransformComponent, CollisionBox>();

		for (auto iter = view.begin(); iter != view.end(); ++iter)
		{

			auto copiedIter = iter;
			copiedIter++;
			for (auto innerIter = copiedIter; innerIter != view.end(); ++innerIter)
			{
				entt::entity e1 = *iter;
				entt::entity e2 = *innerIter;
				Entity entity1 = scene.WrapEntityHandle(e1);
				Entity entity2 = scene.WrapEntityHandle(e2);

				if (entity1.HasComponent<CollisionHandledComponent>())
					break;
				if (entity2.HasComponent<CollisionHandledComponent>())
					continue;

				auto [tc1, cb1] = view.get(e1);
				auto [tc2, cb2] = view.get(e2);

				CollisionBox cb2Collision = cb2.OffsetNew(tc2.position);
				bool collides = cb1.OffsetNew(tc1.position).CollidesWith(&cb2Collision);

				if (collides)
				{
					CollisionEvent eventEntity1{ entity1, false };
					CollisionEvent eventEntity2{ entity2, false };
					scene.GetCollisionDispatcher().Dispatch(eventEntity1, eventEntity2);

					if (eventEntity1.handled)
						entity1.AddComponent<CollisionHandledComponent>();
					if (eventEntity2.handled)
						entity2.AddComponent<CollisionHandledComponent>();
				}

			}
		}
	}

	// Remove all handled collision components
	{
		auto view = scene.getRegistry().view<CollisionHandledComponent>();

		for (auto e : view)
			scene.WrapEntityHandle(e).RemoveComponent<CollisionHandledComponent>();
	}




}
