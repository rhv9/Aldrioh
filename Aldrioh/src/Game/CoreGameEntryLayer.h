#pragma once
#include <Core/Layer.h>

class CoreGameEntryLayer : public Layer
{
public:
	CoreGameEntryLayer(const std::string& name) : Layer(name) {}
	virtual ~CoreGameEntryLayer() override {}
	virtual void OnBegin() override;
};