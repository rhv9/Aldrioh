#pragma once
#include <Scene/Scene.h>

struct LevelArea
{
	glm::vec2 bottomLeft{ 0 };
	glm::vec2 topRight{ 0 };

	float GetWidth() { return topRight.x - bottomLeft.x; }
	float GetHeight() { return topRight.y - bottomLeft.y; }
};

class Level
{
public:

	Level(Scene& scene) : scene(scene) {};
	virtual ~Level() {};
	void virtual OnUpdate(Timestep ts) = 0;
	void virtual OnRender(Timestep ts) = 0;

	virtual void UpdateLevelArea() = 0;
	const LevelArea& GetLevelArea() const { return levelArea; }
	// A reference to the scene from GameLayer
	Scene& scene;

protected:
	LevelArea levelArea;
};