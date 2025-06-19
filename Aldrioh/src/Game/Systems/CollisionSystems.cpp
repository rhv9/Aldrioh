#include "pch.h"
#include "CollisionSystems.h"
#include "HeadersUpdateSystems.h"
#include <Components/Collision.h>

void EntitySystem::CollisionSystem(Timestep ts, Scene& scene)
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

			auto [tc1, cb1] = view.get(e1);
			auto [tc2, cb2] = view.get(e2);

			CollisionBox cb2Collision = cb2.OffsetNew(tc2.position);
			bool collides = cb1.OffsetNew(tc1.position).CollidesWith(&cb2Collision);

			if (collides)
			{
				scene.GetCollisionDispatcher().Dispatch(scene.WrapEntityHandle(e1), scene.WrapEntityHandle(e2));
			}

		}
	}
}
