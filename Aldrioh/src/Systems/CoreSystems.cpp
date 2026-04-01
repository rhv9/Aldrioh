#include <pch.h>
#include "HeadersUpdateSystems.h"
#include "CoreSystems.h"
#include <Math/Math.h>

void EntitySystem::TransformUpdatePrevPosition(Timestep ts, Scene& scene)
{
	auto view = scene.getRegistry().view<TransformComponent>();

	for (entt::entity eHandle : view)
	{
		auto& tc = view.get<TransformComponent>(eHandle);

		tc.prevPosition = tc.position;
	}

}
