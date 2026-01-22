#pragma once

class Scene;
class Entity;

enum class CollectableType : uint8_t
{
	JEWEL1 = 0,
	JEWEL2,
	JEWEL3,
	COIN,
};

struct CCellData
{
	uint8_t x, y;
	CollectableType type;

	static constexpr uint8_t MAX_POINT_VALUE = 255;
};

struct CollectableCell
{
	std::array<CCellData, 32> cellArray;
	int count = 0;

	void AddCollectable(uint8_t x, uint8_t y, CollectableType type);
	void Clear() { count = 0; }
};

struct CollectableMapping
{
	int chunkX = 0, chunkY = 0;
	int cellX = 0, cellY = 0;

	std::string ToString() const;

	bool operator==(const CollectableMapping& other);
};

struct CollectableChunk
{
	static constexpr int SIZE = 16;
	float cellSize = 1.0f;
	Scene& scene;
	std::array<CollectableCell, SIZE* SIZE> grid;

	CollectableChunk(Scene& scene) : scene(scene) {}

	inline CollectableCell& GetCell(int x, int y) { return grid[y * SIZE + x]; }
	inline CollectableCell& GetCell(const CollectableMapping& mapping) { return grid[mapping.cellY * SIZE + mapping.cellX]; }

	void Clear();
};