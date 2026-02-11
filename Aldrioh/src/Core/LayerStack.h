#pragma once
#include "Layer.h"

#include <Events/Events.h>
#include <queue>

namespace Statistics {
	struct EngineStats;
}

class Game;

class LayerStack
{
public:
	LayerStack();
	~LayerStack();

	void Update(Timestep delta);
	void Render(Timestep delta);
	void ImGuiRender(Timestep delta);

	void QueueSwapLayers(Layer* first, Layer* second);
	void QueuePushLayer(Layer* layer);
	void QueuePopLayer(Layer* layer);
	// Does the queued swap layers, if it does do one, will return true
	bool HandleQueuedTasks();

	void OnKeyEvent(KeyEventArg& arg);
	void OnMouseButtonEvent(MouseButtonEventArg& arg);
	void OnMouseScrolledEvent(MouseScrolledEventArg& arg);
	void OnMouseMoveEvent(MouseMoveEventArg& arg);
	void OnWindowCloseEvent(WindowCloseEventArg& arg);
	void OnWindowResizeEvent(WindowResizeEventArg& arg);

	std::vector<Layer*>::iterator begin() { return layerVector.begin(); }
	std::vector<Layer*>::iterator end() { return layerVector.end(); }

private:
	enum class LayerTask : uint8_t {
		NONE = 0, SWAP, PUSH, POP,
	};
	struct LayerTaskData
	{
		LayerTask task = LayerTask::NONE;
		Layer* first = nullptr;
		Layer* second = nullptr;
	};

	void SwapLayers(Layer* first, Layer* second);
	void PushLayer(Layer* layer);
	void PopLayer(Layer* layer);

	std::vector<Layer*> layerVector;
	std::queue<LayerTaskData> layerSwapStack;

	friend Statistics::EngineStats;
	friend Game;
};