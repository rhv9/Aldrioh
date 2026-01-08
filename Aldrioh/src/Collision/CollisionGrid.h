#pragma once

class Scene;

struct Cell
{
	std::array<entt::entity, 10> entities;
	int count = 0;
};

class CollisionGrid
{
public:

	CollisionGrid(Scene& scene);

	Cell& GetCell(const glm::vec2& entityPos);
	void SetSize(int newWidth, int newHeight);

private:
	int width = 1, height = 1;
	float cellSize = 1.0f;
	Scene& scene;
	std::vector<Cell> grid;
};