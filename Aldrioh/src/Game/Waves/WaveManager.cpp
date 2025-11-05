#include <pch.h>
#include "WaveManager.h"

#include <Game/Entity/GameEntityPrefab.h>
#include <Game/Level/Level.h>

#include "ObjectivePreset.h"
#include <Math/Math.h>

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

			WobblyEnemyGroupPrefab enemyManagerPrefab;
			enemyManagerPrefab.count = { 5, 2 };
			enemyManagerPrefab.spacing = { 1.3f, 1.6f };

			enemyManagerPrefab.dirFacing = Math::angleToNormalizedVector(Math::degreesToRad(45));
			enemyManagerPrefab.startPos = glm::vec2{ 0.0f, 3.0f };
			enemyManagerPrefab.distance = 1;
			enemyManagerPrefab.speed = 1;

			Entity enemyManager = enemyManagerPrefab.create(scene);
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

			WobblyEnemyGroupPrefab enemyManagerPrefab;
			enemyManagerPrefab.count = { 10, 4 };
			enemyManagerPrefab.spacing = { 1.3f, 1.6f };

			enemyManagerPrefab.dirFacing = Math::angleToNormalizedVector(Math::degreesToRad(180 + 45));
			enemyManagerPrefab.startPos = glm::vec2{ 0.0f, 3.0f };
			enemyManagerPrefab.distance = 1;
			enemyManagerPrefab.speed = 4;

			Entity enemyManager = enemyManagerPrefab.create(scene);
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

