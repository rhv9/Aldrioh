#include <pch.h>
#include "WaveManager.h"

#include <Game/Entity/GameEntityPrefab.h>
#include <Game/Level/Level.h>

#include "ObjectivePreset.h"

WaveManager::WaveManager(Scene& scene, Level& level) : scene(scene), level(level)
{
}

void WaveManager::Init()
{
	// This is to make it simpler to do the OnInit for first objective
	Objective& firstObj = objectives.emplace_back(Objective{});
	firstObj.complete = true;

	Objective& obj = objectives.emplace_back(Objective{});

	obj.OnInitFunc = [](Objective& obj, Level& level)
		{
			Scene& scene = level.scene;

			EnemyManagerPrefab enemyManagerPrefab;
			Entity enemyManager = enemyManagerPrefab.create(scene);
			for (float y = 1; y < 8; y += 1.3f)
			{
				for (float x = -8; x < 8; x += 2.1f)
				{
					EnemyPrefab enemyPrefab;
					enemyPrefab.enemyManager = enemyManager;
					enemyPrefab.maxHealth = 1;
					enemyPrefab.spawnPos = { x, y };
					enemyPrefab.create(scene);
				}
			}
			obj.entities.push_back(enemyManager);
		};
	obj.OnUpdateFunc = ObjectivePreset::OnUpdate_AllEntitiesDestroyed;
	obj.OnCompleteFunc = [](Objective& obj, Level& level)
		{
			LOG_CORE_INFO("First Wave Complete!!");
		};

	Objective& obj2 = objectives.emplace_back(Objective{});

	obj2.OnInitFunc = [](Objective& obj, Level& level)
		{
			Scene& scene = level.scene;

			EnemyManagerPrefab enemyManagerPrefab;
			Entity enemyManager = enemyManagerPrefab.create(scene);
			for (float y = 1; y < 8; y += 1.3f)
			{
				for (float x = -8; x < 8; x += 2.1f)
				{
					EnemyPrefab enemyPrefab;
					enemyPrefab.enemyManager = enemyManager;
					enemyPrefab.maxHealth = 1;
					enemyPrefab.spawnPos = { x, y };
					enemyPrefab.create(scene);
				}
			}
			obj.entities.push_back(enemyManager);
		};
	obj2.OnUpdateFunc = ObjectivePreset::OnUpdate_AllEntitiesDestroyed;

	obj2.OnCompleteFunc = [](Objective& obj, Level& level)
		{
			LOG_CORE_INFO("First Wave Complete!!");
		};

}

void WaveManager::OnUpdate(Timestep ts)
{
	if (currentObjIndex >= objectives.size())
		return;
	
	Objective& obj = objectives[currentObjIndex];

	if (obj.IsComplete())
	{
		obj.OnCompleteFunc(obj, level);

		++currentObjIndex;
		if (currentObjIndex >= objectives.size())
			return;
		objectives[currentObjIndex].OnInitFunc(objectives[currentObjIndex], level);
	}
	else
		obj.OnUpdateFunc(obj, level);
}

