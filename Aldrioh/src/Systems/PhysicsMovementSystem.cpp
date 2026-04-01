#include <pch.h>
#include "HeadersUpdateSystems.h"
#include "PhysicsMovementSystem.h"
#include <Math/Math.h>

void EntitySystem::PhysicsMovementSystem(Timestep ts, Scene& scene)
{
	{
		auto view = scene.getRegistry().view<MoveControllerComponent, PhysicsMovementComponent>();

		for (entt::entity eHandle : view)
		{
			auto [mcc, pmc] = view.get<MoveControllerComponent, PhysicsMovementComponent>(eHandle);
			pmc.resultantVelocity += mcc.moveDir * mcc.speed;
		}
	}


	{
		auto view = scene.getRegistry().view<TransformComponent, PhysicsMovementComponent>();

		for (entt::entity eHandle : view)
		{
			auto [tc, pmc] = view.get<TransformComponent, PhysicsMovementComponent>(eHandle);
			pmc.resultantVelocity *= 0.95f;
			tc.position += pmc.resultantVelocity * (float)ts;
		}
	}

}
