#include <pch.h>
#include "HeadersUpdateSystems.h"
#include "PhysicsMovementSystem.h"
#include <Math/Math.h>

void EntitySystem::PhysicsMovementSystem(Timestep ts, Scene& scene)
{
	auto view = scene.getRegistry().view<TransformComponent, PhysicsMovementComponent>();

	for (entt::entity eHandle : view)
	{
		auto [tc, pmc] = view.get<TransformComponent, PhysicsMovementComponent>(eHandle);

		tc.position += pmc.resultantVelocity * (float)ts;
	}

}
