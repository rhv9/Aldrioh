#include <pch.h>
#include "CollisionWorld.h"
#include <Scene/Entity.h>

#include <Scene/Components.h>
#include <Collision/Collision.h>

#include <Math/Math.h>

void CollisionWorld::Init(int numOfChunkWidth, int numOfChunkHeight)
{
	width = numOfChunkWidth;
	height = numOfChunkHeight;

	chunks.reserve(width * height);

	for (int i = 0; i < width * height; ++i)
		chunks.emplace_back(scene);
}

PositionMapping CollisionWorld::GetMapping(const glm::vec2& pos)
{
	float chunkSize = chunks[0].SIZE;
	glm::vec2 dividedPos = pos / chunkSize;
	PositionMapping mapping;
	mapping.chunkX = static_cast<int>(dividedPos.x);
	mapping.chunkY = static_cast<int>(dividedPos.y);
	mapping.cellX = static_cast<int>(Math::floatMod(pos.x, chunkSize));
	mapping.cellY = static_cast<int>(Math::floatMod(pos.y, chunkSize));

	return mapping;
}

bool CollisionWorld::FindAndDispatchCollisions(Entity e1, CollisionDispatcher& dispatcher)
{
	bool hasCollided = false;
	glm::vec2 pos1 = glm::vec2(e1.GetTransformComponent().position);
	CollisionBox& cb1 = e1.GetComponent<CollisionBox>();
	glm::vec2 collisionMidPos1 = cb1.OffsetNew({ pos1, 0.0f }).GetMidpoint();

	glm::vec2 maxActualPos = GetMaxActualPosition();

	for (int y = -cellSize; y < cellSize * 2; y += cellSize)
	{
		for (int x = -cellSize; x < cellSize * 2; x += cellSize)
		{
			// offset pos is moving entity pos by cellSize to be able to find the bordering cells, irregardless of chunks, making it easier.
			glm::vec2 offsetPos = collisionMidPos1;
			offsetPos.x += x;
			offsetPos.y += y;

			// position is outside the collision worlds actual pos
			if (offsetPos.x < 0.0f || offsetPos.x >= maxActualPos.x ||
				offsetPos.y < 0.0f || offsetPos.y >= maxActualPos.y)
				continue;

			PositionMapping mapping = GetMapping(offsetPos);
			// get cell
			CollisionChunk& chunk = GetChunk(mapping.chunkX, mapping.chunkY);
			Cell& cell = chunk.GetCell(mapping.cellX, mapping.cellY);

			for (int i = 0; i < cell.count; ++i)
			{
				Entity e2 = scene.WrapEntityHandle(cell.entities[i]);

				if (!e2.IsValid())
					continue;

				if (e1 == e2)
					continue;
				if (e1.HasComponent<CollisionHandledComponent>())
					return true;
				if (e2.HasComponent<CollisionHandledComponent>())
					continue;

				glm::vec3 pos2 = e2.GetTransformComponent().position;
				CollisionBox& cb2 = e2.GetComponent<CollisionBox>();

				CollisionBox cb2Collision = cb2.OffsetNew(pos2);
				bool collides = cb1.OffsetNew(glm::vec3{ pos1, 0.0f }).CollidesWith(&cb2Collision);

				if (collides)
				{
					hasCollided = true;

					CollisionEvent eventEntity1{ e1, false };
					CollisionEvent eventEntity2{ e2, false };
					scene.GetCollisionDispatcher().Dispatch(eventEntity1, eventEntity2);

					if (eventEntity1.handled)
						e1.AddComponent<CollisionHandledComponent>();
					if (eventEntity2.handled)
						e2.AddComponent<CollisionHandledComponent>();
				}
			}
		}
	}

	return hasCollided;
}

glm::vec2 CollisionWorld::GetMaxActualPosition() const
{
	return glm::vec2(width * chunks[0].SIZE * chunks[0].cellSize, height * chunks[0].SIZE * chunks[0].cellSize);
}

std::string PositionMapping::ToString() const
{
	return std::format("Chunk:({},{}), Cell({},{})", chunkX, chunkY, cellX, cellY);
}
