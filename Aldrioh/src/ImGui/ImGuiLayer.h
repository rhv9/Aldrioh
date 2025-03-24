#pragma once
#include "Core/Layer.h"

#include "Events/Events.h"

class ImGuiLayer : public Layer
{
public:
	ImGuiLayer();

	virtual void OnBegin() override;
	virtual void OnUpdate(Timestep delta)override;
	virtual void OnRemove() override;

	void BeginRender();
	void EndRender();

	void BlockEvents(bool val);
	bool isEventsBlocked() { return blockEvents; }

private:
	void OnMouseMove(MouseMoveEventArg& e);
	void OnMousePress(MouseButtonPressedEventArg& e);
	void OnMouseRelease(MouseButtonReleasedEventArg& e);
	void OnKeyPressed(KeyPressedEventArg& e);
	void OnKeyReleased(KeyReleasedEventArg& e);
	void OnMouseScroll(MouseScrolledEventArg& e);

	bool blockEvents = false;
};