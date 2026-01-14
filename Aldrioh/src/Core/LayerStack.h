#pragma once
#include "Layer.h"

namespace Statistics {
	struct EngineStats;
}

class LayerStack
{
public:
	LayerStack();
	~LayerStack();

	void PushLayer(Layer* layer);
	void PopLayer(Layer* layer);

	void Update(Timestep delta);
	void Render(Timestep delta);
	void ImGuiRender(Timestep delta);

	void QueueSwapLayers(Layer* first, Layer* second);
	// Does the queued swap layers, if it does do one, will return true
	bool HandleQueuedTasks();

	std::vector<Layer*>::iterator begin() { return layerVector.begin(); }
	std::vector<Layer*>::iterator end() { return layerVector.end(); }

private:
	void SwapLayers(Layer* first, Layer* second);

	std::vector<Layer*> layerVector;
	std::vector<std::pair<Layer*, Layer*>> layerSwapStack;

	friend Statistics::EngineStats;
};