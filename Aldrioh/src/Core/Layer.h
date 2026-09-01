#pragma once
#include <Events/Events.h>

class Layer
{
public:

	Layer(const std::string& name) : name(name) {}
	virtual ~Layer() {}

	virtual void OnBegin() {}

	virtual void OnUpdate(Timestep delta) {}
	virtual void OnRender(Timestep delta) {}
	virtual void OnImGuiRender(Timestep delta) {}

	virtual void OnRemove() {}

	// This can be overriden, if switching to another layer and you want to handle something before it is swapped.
	void QueueTransitionTo(Layer* layer);
	virtual void OnBeforeQueuedTransitionTo(Layer* layer) {};

	virtual void OnTransitionOut() {};
	virtual void OnTransitionIn() {};

	virtual void OnPushedLayerAboveEvent() {};
	virtual void OnPoppedLayerIntoEvent() {};


	void SetShouldUpdate(bool update) { shouldUpdate = update; }
	void SetShouldRender(bool render) { shouldRender = render; }

	inline bool ShouldUpdate() { return shouldUpdate; }
	inline bool ShouldRender() { return shouldRender; }

	void Initialize();
	inline bool IsInitialized() const { return initialized; }
	inline void SetInitialized(bool val) { initialized = val; }

	virtual void OnKeyEvent(KeyEventArg& arg) {}
	virtual void OnMouseButtonEvent(MouseButtonEventArg& arg) {}
	virtual void OnMouseScrolledEvent(MouseScrolledEventArg& arg) {}
	virtual void OnMouseMoveEvent(MouseMoveEventArg& arg) {}
	virtual void OnWindowCloseEvent(WindowCloseEventArg& arg) {}
	virtual void OnWindowResizeEvent(WindowResizeEventArg& arg) {}

	const std::string& GetName() const { return name; }

private:
	std::string name{""};
	bool shouldUpdate = true, shouldRender = true;
	bool initialized = false;
};
