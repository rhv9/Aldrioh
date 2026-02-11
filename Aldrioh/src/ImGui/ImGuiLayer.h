#pragma once
#include "Core/Layer.h"

#include "Events/Events.h"

class ImGuiLayer : public Layer
{
public:
	ImGuiLayer();
	virtual ~ImGuiLayer() override {}

	virtual void OnBegin() override;
	virtual void OnUpdate(Timestep delta) override {}
	virtual void OnRender(Timestep delta) override {}
	virtual void OnRemove() override;

	void BeginRender();
	void EndRender();

	void BlockEvents(bool val);
	bool isEventsBlocked() { return blockEvents; }

private:
	void OnMouseMove(MouseMoveEventArg& e);
	void OnMouseButton(MouseButtonEventArg& e);
	void OnMouseScroll(MouseScrolledEventArg& e);
	void OnKey(KeyEventArg& e);

	EventCallbackID<MouseMoveEventArg> callbackMouseMoveID;
	EventCallbackID<MouseButtonEventArg> callbackMouseButtonID;
	EventCallbackID<MouseScrolledEventArg> callbackMouseScrollID;
	EventCallbackID<KeyEventArg> callbackKeyID;

	bool blockEvents = false;
};