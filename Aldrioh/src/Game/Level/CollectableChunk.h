#pragma once

class Scene;
class Entity;

enum class CCellType : uint8_t
{
	JEWEL1 = 0,
	JEWEL2,
	JEWEL3,
	COIN,
};

struct CCellData
{
	uint8_t x, y;
	CCellType type;
};

struct CollectableCell
{
	std::array<CCellData, 32> cellArray;
	int count = 0;

	void AddCollectable(uint8_t x, uint8_t y, CCellType type);
};


struct CollectableChunk
{
	static constexpr int SIZE = 16;
	float cellSize = 1.0f;
	Scene& scene;
	std::array<CollectableCell, SIZE* SIZE> grid;

	CollectableChunk(Scene& scene);

	inline CollectableCell& GetCell(int x, int y) { return grid[y * SIZE + x]; }
	void Clear();
};