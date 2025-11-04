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

	void SwapLayers(Layer* first, Layer* second);

	std::vector<Layer*>::iterator begin() { return layerVector.begin(); }
	std::vector<Layer*>::iterator end() { return layerVector.end(); }

private:
	std::vector<Layer*> layerVector;

	friend Statistics::EngineStats;
};