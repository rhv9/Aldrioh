#include <pch.h>
#include "HeadersRenderSystems.h"
#include "UISystems.h"

void EntitySystem::UIManagerUpdateSystem(Timestep ts, Scene& scene)
{
	auto view = scene.getRegistry().view<UIManagerComponent>();

	for (entt::entity e : view)
	{
		UIManagerComponent& uimc = view.get<UIManagerComponent>(e);
		uimc.uiManager->OnUpdate(ts);
	}

}

void EntitySystem::UIManagerRenderSystem(Timestep ts, Scene& scene)
{
	auto view = scene.getRegistry().view<UIManagerComponent>();

	for (entt::entity e : view)
	{
		UIManagerComponent& uimc = view.get<UIManagerComponent>(e);
		uimc.uiManager->OnRender(ts);
	}
}
