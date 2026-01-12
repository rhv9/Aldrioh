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
	void Init(int newWidth, int newHeight);

private:
	int width = 1, height = 1;
	Scene& scene;
	std::vector<Cell> grid;
};