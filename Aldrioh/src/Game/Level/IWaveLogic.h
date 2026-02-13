#pragma once
class Level;

class IWaveLogic
{
public:
	IWaveLogic(Level& level) : level(level) {}
	virtual ~IWaveLogic() {}
	virtual void OnCreate() = 0;
	virtual void OnUpdate(Timestep ts) = 0;
	virtual void OnFinish() = 0;
	
protected:
	Level& level;
};