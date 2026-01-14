#include "pch.h"
#include "Layer.h"

#include <Game.h>

void Layer::QueueTransitionTo(Layer* layer)
{
	Game::Instance().GetLayerStack().QueueSwapLayers(this, layer);
}
