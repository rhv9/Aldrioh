#pragma once

class Scene;

struct Cell
{
	std::array<entt::entity, 16> entities;
	int count = 0;
};


class CollisionChunk
{
public:
	CollisionChunk(Scene& scene);

	Cell& GetCell(const glm::vec2& entityPos);
	float GetCellSize() { return cellSize; }

	static constexpr int SIZE = 16;
private:
	float cellSize = 1.0f;
	Scene& scene;
	std::array<Cell, SIZE * SIZE> grid;
};