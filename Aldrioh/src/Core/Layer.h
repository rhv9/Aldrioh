#pragma once
#include <Events/Events.h>

class Layer
{
public:

	Layer(const std::string& name) : name(name) {}
	virtual ~Layer() {}

	virtual void OnBegin() = 0;

	virtual void OnUpdate(Timestep delta) = 0;
	virtual void OnRender(Timestep delta) = 0;
	virtual void OnImGuiRender(Timestep delta) {}

	virtual void OnRemove() = 0;

	// This can be overriden, if switching to another layer and you want to handle something before it is swapped.
	virtual void QueueTransitionTo(Layer* layer);

	virtual void OnTransitionOut() {};
	virtual void OnTransitionIn() {};

	virtual void OnPushedLayerAboveEvent() {};
	virtual void OnPoppedLayerIntoEvent() {};

	void SetShouldUpdate(bool update) { shouldUpdate = update; }
	void SetShouldRender(bool render) { shouldRender = render; }

	inline bool ShouldUpdate() { return shouldUpdate; }
	inline bool ShouldRender() { return shouldRender; }

	inline bool Initialized() const { return initialized; }
	inline void SetInitialized(bool val) { initialized = val; }

	virtual void OnKeyEvent(KeyEventArg& arg) {}
	virtual void OnMouseButtonEvent(MouseButtonEventArg& arg) {}
	virtual void OnMouseScrolledEvent(MouseScrolledEventArg& arg) {}
	virtual void OnMouseMoveEvent(MouseMoveEventArg& arg) {}
	virtual void OnWindowCloseEvent(WindowCloseEventArg& arg) {}
	virtual void OnWindowResizeEvent(WindowResizeEventArg& arg) {}

private:
	std::string name{""};
	bool shouldUpdate = true, shouldRender = true;
	bool initialized = false;
};
