#pragma once
#include <Collision/CollisionDispatcher.h>

class Scene;

struct CollisionCell
{
	std::array<entt::entity, 16> entities;
	int count = 0;

	void AddEntity(entt::entity handle);
};


struct CollisionPositionMapping
{
	int cellX = 0, cellY = 0;
	std::string ToString() const;
};

class CollisionZone
{
public:

	CollisionZone(Scene& scene) : scene(scene) {}
	void Init(float widthMax, float heightMax, float cellSize);

	bool FindAndDispatchCollisions(Timestep ts, Entity e1, CollisionDispatcher& dispatcher);
	CollisionPositionMapping GetCollisionPositionMapping(const glm::vec2& pos);

	void SetPlayerOffset(const glm::vec2& pos) { playerOffset = pos; }
	glm::vec2 GetPlayerOffset() const { return playerOffset; }
	int GetCenterXCellPos() const { return centerX; }
	int GetCenterYCellPos() const { return centerY; }

	int GetWidth() const { return width; }
	int GetHeight() const { return height; }

	float GetCellSize() const { return cellSize; }


	std::optional<CollisionCell*> GetCell(const CollisionPositionMapping& mapping);

	int GetCollisionCheckCount() const { return collisionCheckCount; }
	void ResetCollisionCheckCount() { collisionCheckCount = 0; }

	void Clear();

private:
	Scene& scene;
	int width = 1, height = 1;
	float cellSize = 1.0f;
	int centerX = 0, centerY = 0;
	glm::vec2 playerOffset{ 0 };
	std::vector<CollisionCell> cellMap;
	int collisionCheckCount = 0;
};