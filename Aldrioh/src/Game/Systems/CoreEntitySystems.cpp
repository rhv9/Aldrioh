#include <pch.h>
#include "CoreEntitySystems.h"
#include <Systems/HeadersUpdateSystems.h>
#include <Game/Components/EntityComponents.h>

#include <Game.h>

#include <Game/GlobalLayers.h>
#include <Game/Components/LevelComponents.h>

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
	LevelComponent& lc = scene.GetFirstEntity<LevelComponent>().GetComponent<LevelComponent>();
	CameraComponent& cameraComponent = scene.GetPrimaryCameraEntity().GetComponent<CameraComponent>();

	const LevelArea& offset = lc.level->GetScreenBorderOffsetByCamera(cameraComponent.cameraController->GetPosition());
	glm::vec2 bottomLeft = offset.bottomLeft;
	glm::vec2 topRight = offset.topRight;

	auto view = scene.getRegistry().view<TransformComponent, EntityTypeComponent>();
	for (entt::entity e : view)
	{
		auto [tc, etc] = view.get<TransformComponent, EntityTypeComponent>(e);

		if (etc.type == EntityTypes::Asteroid)
		{
			// if outside boundaries then queue to delete
			if (tc.position.x < bottomLeft.x || tc.position.y < bottomLeft.y ||
				tc.position.x > topRight.x || tc.position.y > topRight.y)
			{
				scene.WrapEntityHandle(e).QueueDestroy();
			}
		}
	}
}

