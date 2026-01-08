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
	auto view = scene.getRegistry().view<GlobalDumbAIComponent, TransformComponent, MoveComponent>();
	auto& player_mc = scene.GetPlayer()->GetComponent<TransformComponent>();
	for (entt::entity e : view)
	{
		auto [dac, tc, mc] = view.get<GlobalDumbAIComponent, TransformComponent, MoveComponent>(e);

		if (dac.enemyManager.IsValid())
		{
			auto& emc = dac.enemyManager.GetComponent<EnemyManagerComponent>();
			auto& emmc = dac.enemyManager.GetComponent<MoveComponent>();

			mc.speed = emmc.speed;
			mc.updateMoveVec(emmc.moveVec);

			if (dac.firstUpdate)
			{
				dac.firstUpdate = false;
				emc.entityCount++;
			}
		}
	}

	{
		auto view = scene.getRegistry().view<TransformComponent, EnemyManagerComponent, MoveComponent>();

		for (entt::entity e : view)
		{
			auto [tc, emc, mc] = view.get<TransformComponent, EnemyManagerComponent, MoveComponent>(e);

			if (emc.entityCount == 0)
				scene.WrapEntityHandle(e).AddComponent<DestroyEntityComponent>();

			emc.OnUpdateFunc(ts, scene.WrapEntityHandle(e));
		}
	}
}

void EntitySystem::FollowPlayerAISystem(Timestep ts, Scene& scene)
{
	auto view = scene.getRegistry().view<TransformComponent, MoveComponent, VisualComponent, FollowPlayerAIComponent>();

	glm::vec3 playerPos = scene.GetFirstEntity<LevelComponent>().GetComponent<LevelComponent>().level->GetPlayer().GetTransformComponent().position;
	
	for (auto e : view)
	{
		auto [tc, mc, vc, aic] = view.get<TransformComponent, MoveComponent, VisualComponent, FollowPlayerAIComponent>(e);

		glm::vec2 dir = Math::normalizedDirection(glm::vec2{ tc.position }, glm::vec2{ playerPos });
		vc.rotation = Math::angle(dir);
		mc.updateMoveVec(dir);
	}
}
