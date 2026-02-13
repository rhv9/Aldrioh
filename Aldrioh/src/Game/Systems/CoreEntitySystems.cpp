#include <pch.h>
#include "CoreEntitySystems.h"
#include <Systems/HeadersUpdateSystems.h>
#include <Game/Components/EntityComponents.h>

#include <Game.h>

#include <Game/GlobalLayers.h>
#include <Game/Components/LevelComponents.h>

#include <Game/Entity/GameEntities.h>

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
				if (aic.enemyManager.IsValid())
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

void EntitySystem::RotationSystem(Timestep ts, Scene& scene)
{
	uint32_t tickCount = Game::Instance().GetTickCount();
	auto view = scene.getRegistry().view<RotationComponent, VisualComponent>();
	for (entt::entity e : view)
	{
		auto [rc, vc] = view.get<RotationComponent, VisualComponent>(e);

		if ((tickCount % rc.skipTicks) == 0)
		{
			if (rc.shouldRotateTo)
				vc.rotation = rc.angle;
			else
				vc.rotation += rc.angle * ts * rc.skipTicks;
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
				entity.QueueDestroy();
		}
	}
}

void EntitySystem::DeleteEnemyOutsideScreenSystem(Timestep ts, Scene& scene)
{
	// Only run every second
	if (Game::Instance().GetTickCount() % Game::Instance().GetTicksTarget() != 0)
		return;

	LevelComponent& lc = scene.GetFirstEntity<LevelComponent>().GetComponent<LevelComponent>();
	BoundingArea deathArea = lc.level->GetDeathArea();
	glm::vec2 bottomLeft = deathArea.bottomLeft;
	glm::vec2 topRight = deathArea.topRight;

	auto view = scene.getRegistry().view<TransformComponent, EntityTypeComponent>();
	for (entt::entity e : view)
	{
		auto [tc, etc] = view.get<TransformComponent, EntityTypeComponent>(e);

		if (etc.typeId == EnemyEntityTypes::Asteroid->entityId || etc.typeId.category == EntityCategory::Enemy)
		{
			// if outside boundaries then queue to delete
			if (tc.position.x < bottomLeft.x || tc.position.y < bottomLeft.y ||
				tc.position.x > topRight.x || tc.position.y > topRight.y)
			{
				Entity eWrapped = scene.WrapEntityHandle(e);
				if (eWrapped.HasComponent<CoreEnemyStateComponent>())
					eWrapped.GetComponent<CoreEnemyStateComponent>().addScoreOnDeath = false;

				// TODO: I don't think this should do it for all wiped enemies since it may not be an enemy
				lc.level->GetLevelStats().onEnemyWipedByBorder(etc.typeId);
				eWrapped.QueueDestroy();
			}
		}
	}
}

