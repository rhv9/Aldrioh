#include "pch.h"
#include "EnemyAISystems.h"
#include <Scene/Scene.h>
#include <Scene/Entity.h>
#include "HeadersUpdateSystems.h"

#include <Core/Platform.h>
#include <Math/Math.h>

void EntitySystem::DumbAISystem(Timestep ts, Scene& scene)
{
	// DumbAIComponent
	auto view = scene.getRegistry().view<DumbAIComponent, TransformComponent, MoveComponent>();
	auto& player_mc = scene.GetPlayer()->GetComponent<TransformComponent>();
	for (entt::entity e : view)
	{
		auto [dac, tc, mc] = view.get<DumbAIComponent, TransformComponent, MoveComponent>(e);
		float elapsedTime = Platform::GetElapsedTime();

		if (tc.position.x < dac.startPos.x - dac.distance)
			dac.move = 1;

		if (tc.position.x > dac.startPos.x + dac.distance)
			dac.move = -1;

		LOG_CORE_INFO("dac move: {}", dac.move);
		mc.updateMoveVec({dac.move, 0 });
	}
}
