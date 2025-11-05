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
				if (e.Valid())
					complete = false;
			}

			obj.complete = complete;
		};

}