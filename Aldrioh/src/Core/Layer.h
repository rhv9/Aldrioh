#pragma once

class Layer
{
public:

	Layer() = default;

	virtual void OnBegin() = 0;

	virtual void OnUpdate(Timestep delta) = 0;
	virtual void OnImGuiRender(Timestep delta) {}

	virtual void OnRemove() = 0;

	void TransitionTo(Layer* layer);

	virtual void OnTransitionOut() {};
	virtual void OnTransitionIn() {};

private:

};
