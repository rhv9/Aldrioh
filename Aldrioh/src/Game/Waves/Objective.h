#pragma once
#include <Scene/Entity.h>

class Level;
class Scene;

class Objective
{
public:
	Objective();
	void Init(Level& level);

	bool IsComplete() const { return complete; }

	std::function<void(Objective&, Level&)> OnInitFunc;
	std::function<void(Objective&, Level&)> OnUpdateFunc;
	std::function<void(Objective&, Level&)> OnCompleteFunc;
	bool complete = false;

	std::vector<Entity> entities;
};