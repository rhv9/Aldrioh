#pragma once

class Scene;
class Entity;

#define EntityPrefabCreateOverride virtual Entity create(Scene& scene) override

// Interface for create
struct EntityPrefab
{
	virtual Entity create(Scene& scene) = 0;
};