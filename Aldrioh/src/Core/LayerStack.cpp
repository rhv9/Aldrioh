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
	if (!layer->Initialized())
	{
		layer->OnBegin();
		layer->SetInitialized(true);
	}
	
	if (layerVector.size() != 0)
		layerVector[layerVector.size() - 1]->OnPushedLayerAboveEvent();

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
		if (layerVector.size() != 0)
			layerVector[layerVector.size() - 1]->OnPoppedLayerIntoEvent();
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

void LayerStack::QueueSwapLayers(Layer* first, Layer* second)
{
	layerSwapStack.push_back({ first, second });
}

bool LayerStack::HandleQueuedTasks()
{
	ASSERT(layerSwapStack.size() < 2, "Why is there two transitions queued? Maybe it is valid");
	bool doneSwap = false;
	for (std::pair<Layer*, Layer*>& pair : layerSwapStack)
	{
		doneSwap = true;
		SwapLayers(pair.first, pair.second);
	}
	if (doneSwap)
		layerSwapStack.clear();
	return doneSwap;
}

