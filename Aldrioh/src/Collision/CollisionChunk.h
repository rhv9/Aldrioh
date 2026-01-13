#pragma once

class Scene;
class Entity;

struct Cell
{
	std::array<entt::entity, 16> entities;
	int count = 0;

	void AddEntity(entt::entity handle);
};


struct CollisionChunk
{
	static constexpr int SIZE = 16;
	float cellSize = 1.0f;
	Scene& scene;
	std::array<Cell, SIZE * SIZE> grid;

	CollisionChunk(Scene& scene);

	inline Cell& GetCell(int x, int y) { return grid[y * SIZE + x]; }
	void Clear();
};