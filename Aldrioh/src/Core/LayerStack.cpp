#include "pch.h"
#include "LayerStack.h"

LayerStack::LayerStack()
{
}

LayerStack::~LayerStack()
{
	// TODO Need to think about this
	//for (Layer* layer : layerVector)
	//{
	//	layer->OnRemove();
	//	delete layer;
	//}
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
			if (!second->Initialized())
				second->OnBegin();
			second->OnTransitionIn();
			return;
		}

	}
	LOG_CORE_CRITICAL("Layer does not exist in layerstack!");
}

void LayerStack::QueueSwapLayers(Layer* first, Layer* second)
{
	layerSwapStack.push({ LayerTask::SWAP, first, second });
}

void LayerStack::QueuePushLayer(Layer* layer)
{
	layerSwapStack.push({ LayerTask::PUSH, layer, nullptr });
}

void LayerStack::QueuePopLayer(Layer* layer)
{
	layerSwapStack.push({ LayerTask::POP, layer, nullptr });
}

bool LayerStack::HandleQueuedTasks()
{
	bool doneSwap = false;
	while (!layerSwapStack.empty())
	{
		doneSwap = true;
		LayerTaskData& job = layerSwapStack.front();

		if (job.task == LayerTask::SWAP)
			SwapLayers(job.first, job.second);
		else if (job.task == LayerTask::PUSH)
			PushLayer(job.first);
		else if (job.task == LayerTask::POP)
			PopLayer(job.first);
		
		layerSwapStack.pop();
	}

	return doneSwap;
}

void LayerStack::OnKeyEvent(KeyEventArg& arg)
{
	for (auto it = layerVector.rbegin(); it != layerVector.rend(); ++it)
	{
		(*it)->OnKeyEvent(arg);
		if (arg.isHandled)
			break;
	}
}

void LayerStack::OnMouseButtonEvent(MouseButtonEventArg& arg)
{
	for (auto it = layerVector.rbegin(); it != layerVector.rend(); ++it)
	{
		(*it)->OnMouseButtonEvent(arg);
		if (arg.isHandled)
			break;
	}
}

void LayerStack::OnMouseScrolledEvent(MouseScrolledEventArg& arg)
{
	for (auto it = layerVector.rbegin(); it != layerVector.rend(); ++it)
	{
		(*it)->OnMouseScrolledEvent(arg);
		if (arg.isHandled)
			break;
	}
}

void LayerStack::OnMouseMoveEvent(MouseMoveEventArg& arg)
{
	for (auto it = layerVector.rbegin(); it != layerVector.rend(); ++it)
	{
		(*it)->OnMouseMoveEvent(arg);
		if (arg.isHandled)
			break;
	}
}

void LayerStack::OnWindowCloseEvent(WindowCloseEventArg& arg)
{
	for (auto it = layerVector.rbegin(); it != layerVector.rend(); ++it)
	{
		(*it)->OnWindowCloseEvent(arg);
		if (arg.isHandled)
			break;
	}
}

void LayerStack::OnWindowResizeEvent(WindowResizeEventArg& arg)
{
	for (auto it = layerVector.rbegin(); it != layerVector.rend(); ++it)
	{
		(*it)->OnWindowResizeEvent(arg);
		if (arg.isHandled)
			break;
	}
}

