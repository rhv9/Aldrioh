#include "pch.h"
#include "LayerStack.h"

LayerStack::LayerStack()
{
}

LayerStack::~LayerStack()
{
	for (Layer* layer : layerVector)
	{
		layer->OnRemove();
		delete layer;
	}
}

void LayerStack::PushLayer(Layer* layer)
{
	layerVector.push_back(layer);
}

void LayerStack::PopLayer(Layer* layer)
{
	std::vector<Layer*>::iterator it = std::find(layerVector.begin(), layerVector.end(), layer);

	if (it != layerVector.end())
	{
		layer->OnRemove();
		layerVector.erase(it);
	}
}
