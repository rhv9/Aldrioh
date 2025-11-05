#pragma once
#include "Objective.h"

namespace ObjectivePreset {

	static std::function<void(Objective&, Level&)> Empty = [](Objective&, Level&) {};

	// Updates
	extern std::function<void(Objective&, Level&)> OnUpdate_AllEntitiesDestroyed;

}