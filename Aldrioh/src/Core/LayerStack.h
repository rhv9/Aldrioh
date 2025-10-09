#pragma once
#include "Layer.h"

class LayerStack
{
public:
	LayerStack();
	~LayerStack();

	void PushLayer(Layer* layer);
	void PopLayer(Layer* layer);

	template<typename T, typename U>
	void SwapLayer()
	{
		for (Layer* layer : layerVector)
		{
			if (layer->ID == T::ID)
			{

			}
		}
	}


	std::vector<Layer*>::iterator begin() { return layerVector.begin(); }
	std::vector<Layer*>::iterator end() { return layerVector.end(); }

private:
	std::vector<Layer*> layerVector;
};