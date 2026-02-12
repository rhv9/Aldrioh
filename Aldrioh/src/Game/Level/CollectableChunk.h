#pragma once

class Scene;
class Entity;

enum class CollectableType : uint8_t
{
	NONE = 0,
	JEWEL1,
	JEWEL2,
	JEWEL3,
	COIN,
};

struct CollectableItem
{
	struct RenderData
	{
		int spriteId = 0;
		glm::vec4 colour{ 0 };
		glm::vec2 size{ 1.0f };
	};

	uint8_t x, y;
	CollectableType type;

	glm::vec2 GetFloatOffset() const;
	void SetPosOnDecimalValue(const glm::vec2 pos);
	RenderData GetRenderData() const;

	static constexpr uint8_t MAX_POINT_VALUE = 255;
};

struct CollectableMapping
{
	int chunkX = 0, chunkY = 0;
	int cellX = 0, cellY = 0;

	std::string ToString() const;

	bool operator==(const CollectableMapping& other);
};

struct CachedCollectableBlock
{
	std::vector<CollectableItem> cellVector;
	int x = 0, y = 0;

	CachedCollectableBlock() = default;
	CachedCollectableBlock(int x, int y) : x(x), y(y) {}
};

struct CollectableBlock
{
	std::array<CollectableItem, 32> cellArray;
	int count = 0;

	void AddCollectable(const glm::vec2& untrimmedPos, CollectableType type);
	void AddCollectable(uint8_t xOffset, uint8_t yOffset, CollectableType type);

	void Clear() { count = 0; }
};

class CollectableChunk
{
public:
	static constexpr int SIZE = 8;
	static constexpr float REFRESH_TIMER = 5;

	CollectableChunk() = default;
	CollectableChunk(const std::vector<CachedCollectableBlock>& blocks);

	inline CollectableBlock& GetBlock(int x, int y) { return grid[y * SIZE + x]; }
	inline CollectableBlock& GetBlock(const CollectableMapping& mapping) { return grid[mapping.cellY * SIZE + mapping.cellX]; }

	void Render(const glm::vec2& offset);
	void Clear();

	void ResetTimer() { timeRemaining = REFRESH_TIMER; }
	void RemoveTime(Timestep ts) { timeRemaining -= ts; }
	bool ShouldUnload() const { return timeRemaining < 0.0f; };
	float GetTimeRemaining() const { return timeRemaining; }
private:
	float cellSize = 1.0f;
	std::array<CollectableBlock, SIZE * SIZE> grid;
	float timeRemaining = REFRESH_TIMER;
};