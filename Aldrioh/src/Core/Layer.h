#pragma once

class Layer
{
public:

	Layer() = default;

	virtual void OnBegin() = 0;

	virtual void OnUpdate(Timestep delta) = 0;
	virtual void OnRender(Timestep delta) = 0;
	virtual void OnImGuiRender(Timestep delta) {}

	virtual void OnRemove() = 0;

	void TransitionTo(Layer* layer);

	virtual void OnTransitionOut() {};
	virtual void OnTransitionIn() {};

	void SetShouldUpdate(bool update) { shouldUpdate = update; }
	void SetShouldRender(bool render) { shouldRender = render; }

	inline bool ShouldUpdate() { return shouldUpdate; }
	inline bool ShouldRender() { return shouldRender; }

private:

	bool shouldUpdate = true, shouldRender = true;
};
