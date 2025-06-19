#pragma once

class Level
{
public:

	Level() = default;
	virtual ~Level() {};
	void virtual OnUpdate(Timestep ts) = 0;
	void virtual OnRender(Timestep ts) = 0;

};