#include "pch.h"
#include "LevelSystems.h"
#include <Scene/Scene.h>
#include <Scene/Entity.h>
#include <Scene/Components.h>
#include <Game/Components/LevelComponents.h>

void EntitySystem::LevelUpdateSystem(Timestep ts, Scene& scene)
{
	auto view = scene.getRegistry().view<LevelComponent>();

	for (entt::entity e : view)
	{
		auto& lc = view.get<LevelComponent>(e);
		lc.level->OnUpdate(ts);
	}
}

void EntitySystem::LevelRenderSystem(Timestep ts, Scene& scene)
{
	auto view = scene.getRegistry().view<LevelComponent>();

	for (entt::entity e : view)
	{
		auto& lc = view.get<LevelComponent>(e);
		lc.level->OnRender(ts);
	}
}

