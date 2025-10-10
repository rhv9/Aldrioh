#pragma once
#include "Layer.h"

// These functions must be implemented on any game so that a layer is added.
namespace LayerInitialiser {

	// Here you return the first layer that is pushed
	Layer* PushFirstLayer();

	// Here you create an array of all the layers you will use in the app. Only reason it is on an array is so that all OnBegin() are ran.
	std::vector<Layer*> OtherLayers();

}
