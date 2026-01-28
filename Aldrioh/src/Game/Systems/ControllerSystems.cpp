#include "pch.h"
#include "ControllerSystems.h"
#include <Systems/HeadersUpdateSystems.h>
#include <Game/Components/EntityComponents.h>

void EntitySystem::ControllerSystems(Timestep ts, Scene& scene)
{
	auto view = scene.getRegistry().view<BezierPathComponent, ItemAnimationControllerComponent>();

	for (entt::entity eHandle : view)
	{
		auto [bezier, iacc] = view.get<BezierPathComponent, ItemAnimationControllerComponent>(eHandle);
		bezier.p2 = scene.GetPlayer().GetTransformComponent().position;
	}
}