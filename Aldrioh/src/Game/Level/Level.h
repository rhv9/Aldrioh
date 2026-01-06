#pragma once
#include <Scene/Scene.h>
#include <Scene/Entity.h>

#include <Game/Waves/WaveManager.h>

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
	// Gives the bottom left and top right offset for screen to world position
	const LevelArea& GetScreenBorderOffset() const { return levelArea; }
	const LevelArea& GetScreenBorderOffsetByCamera(const glm::vec2& offset);

	void UpdateScore(float newScore);

	// A reference to the scene from GameLayer
	Scene& scene;
protected:
	LevelArea levelArea;
	Entity camera;
	WaveManager waveManager;
};

