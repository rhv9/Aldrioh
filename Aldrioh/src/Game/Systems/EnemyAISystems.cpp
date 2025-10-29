#include "pch.h"
#include "EnemyAISystems.h"
#include <Scene/Scene.h>
#include <Scene/Entity.h>
#include <Systems/HeadersUpdateSystems.h>
#include <Game/Components/EntityComponents.h>

#include <Core/Platform.h>
#include <Math/Math.h>

void EntitySystem::DumbAISystem(Timestep ts, Scene& scene)
{

	// DumbAIComponent
	auto view = scene.getRegistry().view<GlobalDumbAIComponent, TransformComponent, MoveComponent>();
	auto& player_mc = scene.GetPlayer()->GetComponent<TransformComponent>();
	for (entt::entity e : view)
	{
		auto [dac, tc, mc] = view.get<GlobalDumbAIComponent, TransformComponent, MoveComponent>(e);

		if (dac.enemyManager.Valid())
		{
			auto& emc = dac.enemyManager.GetComponent<EnemyManagerComponent>();
			mc.updateMoveVec({emc.move * emc.speed, -0.3f });
		
			if (dac.firstUpdate)
			{
				dac.firstUpdate = false;
				emc.entityCount++;
			}
		}
	}

	{
		auto view = scene.getRegistry().view<EnemyManagerComponent, MoveComponent>();

		for (entt::entity e : view)
		{
			float elapsedTime = Platform::GetElapsedTime();

			auto [emc, mc] = view.get<EnemyManagerComponent, MoveComponent>(e);
			auto& tc = scene.getRegistry().get<TransformComponent>(e);

			if (emc.entityCount == 0)
				scene.WrapEntityHandle(e).AddComponent<DestroyEntityComponent>();

			if (emc.startTime == -1)
			{
				emc.startTime = elapsedTime;
				emc.startX = tc.position.x;
			}

			if (tc.position.x > emc.startX + emc.distance)
				emc.move = -1;
			else if (tc.position.x < emc.startX - emc.distance)
				emc.move = 1;


			mc.updateMoveVec({ emc.move * emc.speed, 0 });
		}
	}


}
