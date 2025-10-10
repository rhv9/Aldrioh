#include "pch.h"
#include "Layer.h"

#include <Game.h>

void Layer::TransitionTo(Layer* layer)
{
	Game::Instance().GetLayerStack().SwapLayers(this, layer);
}
