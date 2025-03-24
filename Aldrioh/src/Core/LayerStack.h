#pragma once
#include "Layer.h"

class LayerStack
{
public:
	LayerStack();
	~LayerStack();

	void PushLayer(Layer* layer);
	void PopLayer(Layer* layer);

	std::vector<Layer*>::iterator begin() { return layerVector.begin(); }
	std::vector<Layer*>::iterator end() { return layerVector.end(); }

private:
	std::vector<Layer*> layerVector;
};