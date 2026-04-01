#include "pch.h"
#include "EnemyAISystems.h"
#include <Scene/Scene.h>
#include <Scene/Entity.h>
#include <Systems/HeadersUpdateSystems.h>
#include <Game/Components/EntityComponents.h>

#include <Core/Platform.h>
#include <Math/Math.h>
#include <Game/Components/LevelComponents.h>

void EntitySystem::DumbAISystem(Timestep ts, Scene& scene)
{
	// DumbAIComponent
	auto view = scene.getRegistry().view<GlobalDumbAIComponent, TransformComponent, MoveControllerComponent>();
	auto& player_mc = scene.GetPlayer().GetComponent<TransformComponent>();
	for (entt::entity e : view)
	{
		auto [dac, tc, mcc] = view.get<GlobalDumbAIComponent, TransformComponent, MoveControllerComponent>(e);

		if (dac.enemyManager.IsValid())
		{
			auto& emc = dac.enemyManager.GetComponent<EnemyManagerComponent>();
			auto& emcc = dac.enemyManager.GetComponent<MoveControllerComponent>();

			mcc.speed = emcc.speed;
			mcc.moveDir = emcc.moveDir;

			if (dac.firstUpdate)
			{
				dac.firstUpdate = false;
				emc.entityCount++;
			}
		}
	}

	{
		auto view = scene.getRegistry().view<TransformComponent, EnemyManagerComponent, MoveControllerComponent>();

		for (entt::entity e : view)
		{
			auto [tc, emc, mcc] = view.get<TransformComponent, EnemyManagerComponent, MoveControllerComponent>(e);

			if (emc.entityCount == 0)
				scene.WrapEntityHandle(e).AddComponent<DestroyEntityComponent>();

			emc.OnUpdateFunc(ts, scene.WrapEntityHandle(e));
		}
	}
}

void EntitySystem::FollowPlayerAISystem(Timestep ts, Scene& scene)
{
	auto view = scene.getRegistry().view<TransformComponent, MoveControllerComponent, VisualComponent, FollowPlayerAIComponent>();

	glm::vec2 playerPos = scene.GetFirstEntity<LevelComponent>().GetComponent<LevelComponent>().level->GetPlayer().GetTransformComponent().position;
	
	for (auto e : view)
	{
		auto [tc, mcc, vc, aic] = view.get<TransformComponent, MoveControllerComponent, VisualComponent, FollowPlayerAIComponent>(e);

		glm::vec2 dir = Math::normalizedDirection(glm::vec2{ tc.position }, glm::vec2{ playerPos });
		vc.rotation = Math::angle(dir) - Math::PI / 2.0f;
		mcc.moveDir = dir;
	}
}
