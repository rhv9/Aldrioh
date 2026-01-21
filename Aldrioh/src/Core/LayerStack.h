#pragma once
#include "Layer.h"

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
	std::vector<LayerTaskData> layerSwapStack;

	friend Statistics::EngineStats;
	friend Game;
};