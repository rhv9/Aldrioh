#pragma once
#include <Scene/Scene.h>
#include <Scene/Entity.h>

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

	Level(Scene& scene);
	~Level();
	void OnUpdate(Timestep ts);
	void OnRender(Timestep ts);

	void UpdateLevelArea();
	const LevelArea& GetLevelArea() const { return levelArea; }

	void UpdateScore(float newScore);

	// A reference to the scene from GameLayer
	Scene& scene;
protected:
	LevelArea levelArea;
	Entity camera;

};

