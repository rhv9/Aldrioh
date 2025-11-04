#include <pch.h>
#include "SpawnerTile.h"
#include <Game/SpriteCollection.h>

#include <Graphics/RenderQueue.h>

#include <Game/RenderDepth.h>
#include <Math/Math.h>

#include <Game/Level/TestLevel.h>

SpawnerTile::SpawnerTile(const int floorSpriteId, EntityType entityType) : floorSpriteId(floorSpriteId), entityType(entityType),
spawnArea(glm::vec2{7, 7})
{
	spawnerSpriteId = Sprites::spawner;
	remainingTime = Math::Random::linearInt(3, 10);
}

SpawnerTile::~SpawnerTile()
{
}

void SpawnerTile::OnUpdate(Timestep ts, const TileMetaData& metadata)
{
	if (remainingTime <= 0)
	{
		remainingTime = Math::Random::linearFloat(3.0f, 5.0f);
		glm::vec2 spawnPos{
			Math::Random::linearFloat(-(spawnArea.x / 2.0f), spawnArea.x / 2.0f),
			Math::Random::linearFloat(-(spawnArea.y / 2.0f), spawnArea.y / 2.0f)
		};

		spawnPos += metadata.pos;

		//static_cast<TestLevel*>(metadata.level)->CreateBoss(spawnPos);
	}
	else
		remainingTime -= ts;
}

void SpawnerTile::OnRender(Timestep ts, const TileMetaData& metadata)
{
	glm::vec3 renderPos = { metadata.pos.x * 1.0f, metadata.pos.y * 1.0f, RenderDepth::TILE };
	RenderQueue::EnQueue(RenderLayer::ZERO, renderPos, floorSpriteId);
	RenderQueue::EnQueue(RenderLayer::ZERO, renderPos, spawnerSpriteId);
}
