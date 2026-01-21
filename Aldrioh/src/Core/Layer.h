#pragma once

class Layer
{
public:

	Layer(const std::string& name) : name(name) {}

	virtual void OnBegin() = 0;

	virtual void OnUpdate(Timestep delta) = 0;
	virtual void OnRender(Timestep delta) = 0;
	virtual void OnImGuiRender(Timestep delta) {}

	virtual void OnRemove() = 0;

	void QueueTransitionTo(Layer* layer);

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

private:
	std::string name{""};
	bool shouldUpdate = true, shouldRender = true;
	bool initialized = false;
};
