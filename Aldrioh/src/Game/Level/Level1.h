#pragma once
#include "Level.h"


class Level1 : public Level
{
public:

	Level1(Scene& scene);
	virtual ~Level1() override;
	void virtual OnUpdate(Timestep ts) override;
	void virtual OnRender(Timestep ts) override;

	void UpdateScore(float newScore);
};