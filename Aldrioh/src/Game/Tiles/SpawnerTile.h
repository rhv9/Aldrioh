#pragma once
#include "Tiles.h"
#include <Game/Entity/EntityType.h>

class SpawnerTile : public Tiles
{
public:
	SpawnerTile(const int floorSpriteId, EntityType entityType);
	virtual ~SpawnerTile() override;

	virtual void OnUpdate(Timestep ts, const TileMetaData& metadata) override;
	virtual void OnRender(Timestep ts, const TileMetaData& metadata) override;

	const int GetFloorSpriteId() const { return floorSpriteId; }

private:
	int floorSpriteId, spawnerSpriteId;
	float remainingTime = 0.0f;
	glm::vec2 spawnArea;
	EntityType entityType;

};