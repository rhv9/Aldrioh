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

			if (entity.HasComponent<OnDestroyComponent>())
				entity.GetComponent<OnDestroyComponent>().onDeathFunc(entity);

			if (entity.HasComponent<GlobalDumbAIComponent>())
			{
				GlobalDumbAIComponent& aic = entity.GetComponent<GlobalDumbAIComponent>();
				if (aic.enemyManager.Valid())
				{
					EnemyManagerComponent& emc = aic.enemyManager.GetComponent<EnemyManagerComponent>();
					emc.entityCount--;
				}
				
			}

			scene.DestroyEntity(entity);
		}
	}
}

void EntitySystem::HealthSystem(Timestep ts, Scene& scene)
{
	{
		auto view = scene.getRegistry().view<HealthComponent>();

		for (entt::entity e : view)
		{
			Entity entity = scene.WrapEntityHandle(e);
			HealthComponent& hc = view.get<HealthComponent>(e);

			if (hc.health <= 0.0f)
				entity.AddComponent<DestroyEntityComponent>();
		}
	}
}

