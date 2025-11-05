#include "pch.h"
#include "Objective.h"
#include <Game/Level/Level.h>

Objective::Objective()
{
	auto empty = [](Objective&, Level&) {};
	OnInitFunc = empty;
	OnUpdateFunc = empty;
	OnCompleteFunc = empty;
}

void Objective::Init(Level& level)
{
	OnInitFunc(*this, level);
}
