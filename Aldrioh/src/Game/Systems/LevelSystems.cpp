#include "pch.h"
#include "LevelSystems.h"
#include "HeadersUpdateSystems.h"
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


void EntitySystem::ScoreSystems(Timestep ts, Scene& scene)
{
	{
		auto view = scene.getRegistry().view<AddScoreComponent>();
		for (auto e : view)
		{
			AddScoreComponent& asc = view.get<AddScoreComponent>(e);
		
			{
				auto view = scene.getRegistry().view<ScoreComponent>();
				for (auto e : view)
				{
					ScoreComponent& sc = view.get<ScoreComponent>(e);
					sc.score += asc.points;
					sc.updated = true;
				}
			}

			scene.WrapEntityHandle(e).Destroy();
		}
	}

	{
		auto view = scene.getRegistry().view<ScoreComponent>();
		for (auto e : view)
		{
			ScoreComponent& sc = view.get<ScoreComponent>(e);
			if (sc.updated)
			{
				sc.updated = false;
				sc.onScoreUpdate(sc.score);
			}
		}
	}
}


