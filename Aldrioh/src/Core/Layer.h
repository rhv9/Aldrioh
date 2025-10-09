#pragma once

class Layer
{
public:

	Layer() = default;

	virtual void OnBegin() = 0;

	virtual void OnUpdate(Timestep delta) = 0;
	virtual void OnImGuiRender(Timestep delta) {}

	virtual void OnRemove() = 0;

	template<typename T>
	void TransitionTo()
	{

	}

	virtual void OnTransitionOut() {};
	virtual void OnTransitionIn() {};

private:

};
