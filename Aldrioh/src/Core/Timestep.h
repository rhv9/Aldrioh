#pragma once


class Timestep
{
public:
	Timestep(float time) : time(time) {}

	operator float()
	{
		return time;
	}

	float GetSeconds() const { return time; }
	float GetMilliSeconds() const { return time * 1000; }

private:
	float time;
};