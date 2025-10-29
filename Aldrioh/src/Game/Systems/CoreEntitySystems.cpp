#include <pch.h>
#include "CoreEntitySystems.h"
#include <Systems/HeadersUpdateSystems.h>
#include <Game/Components/EntityComponents.h>

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

	// Visual Hit System

	{
		auto view = scene.getRegistry().view<CoreEnemyStateComponent, VisualComponent>();
		for (entt::entity e : view)
		{
			auto [cesc, vc] = view.get<CoreEnemyStateComponent, VisualComponent>(e);

			switch (cesc.hitVisualState)
			{
			case HitVisualState::JUST_HIT:
				cesc.hitvisualOriginalColour = vc.colour;
				vc.colour = glm::vec4{ 1 };
				vc.flags = 1;
				cesc.hitVisualState = HitVisualState::COUNTING_DOWN;
				break;
			case HitVisualState::COUNTING_DOWN:
				if (cesc.hitVisualTimer <= 0.0f)
					cesc.hitVisualState = HitVisualState::FINISH;
				cesc.hitVisualTimer -= ts;
				break;
			case HitVisualState::FINISH:
				vc.colour = cesc.hitvisualOriginalColour;
				vc.flags = 0;
				cesc.hitVisualState = HitVisualState::NORMAL;
				break;
			}
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

