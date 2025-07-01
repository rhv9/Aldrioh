#include <pch.h>
#include "CoreEntitySystems.h"
#include "HeadersUpdateSystems.h"

void EntitySystem::CoreEntitySystems(Timestep ts, Scene& scene)
{
	// Destroy Entity
	{
		auto view = scene.getRegistry().view<DestroyEntityComponent>();

		for (entt::entity e : view)
		{
			Entity entity = scene.WrapEntityHandle(e);
			scene.DestroyEntity(entity);
		}
	}
}
