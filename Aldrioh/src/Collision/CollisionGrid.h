#pragma once
#include <Scene/Scene.h>

struct Cell
{
	std::array<entt::entity, 10> entities;
	int count = 0;
};

class CollisionSystem
{
public:
	CollisionSystem(Scene& scene);

	Cell& GetCell(const glm::vec2& entityPos);

private:
	int width, height;
	float cellSize = 1.0f;
	Scene& scene;
	std::vector<Cell> grid;
};