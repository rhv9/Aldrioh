#include <pch.h>
#include "ObjectivePreset.h"

namespace ObjectivePreset {

	// Updates
	std::function<void(Objective&, Level&)> OnUpdate_AllEntitiesDestroyed =
		[](Objective& obj, Level& level)
		{
			bool complete = true;
			for (Entity& e : obj.entities)
			{
				if (e.IsValid())
					complete = false;
			}

			obj.complete = complete;
		};

}