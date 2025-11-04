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
	layer->OnTransitionIn();
}

void LayerStack::PopLayer(Layer* layer)
{
	std::vector<Layer*>::iterator it = std::find(layerVector.begin(), layerVector.end(), layer);

	if (it != layerVector.end())
	{
		layer->OnTransitionOut();
		layerVector.erase(it);
	}
}

void LayerStack::Update(Timestep delta)
{
	for (Layer* layer : layerVector)
	{
		if (layer->ShouldUpdate())
			layer->OnUpdate(delta);
	}
}

void LayerStack::Render(Timestep delta)
{
	for (Layer* layer : layerVector)
	{
		if (layer->ShouldRender())
			layer->OnRender(delta);
	}
}

void LayerStack::ImGuiRender(Timestep delta)
{
	for (Layer* layer : layerVector)
	{
		layer->OnImGuiRender(delta);
	}
}

void LayerStack::SwapLayers(Layer* first, Layer* second)
{
	for (int i = 0; i < layerVector.size(); ++i)
	{
		if (layerVector[i] == first)
		{
			first->OnTransitionOut();
			layerVector[i] = second;
			second->OnTransitionIn();
			return;
		}

	}
	LOG_CORE_CRITICAL("Layer does not exist in layerstack!");

}

