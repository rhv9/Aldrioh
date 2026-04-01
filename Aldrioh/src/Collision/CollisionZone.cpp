#include <pch.h>
#include "CollisionZone.h"
#include <Scene/Entity.h>

#include <Scene/Components.h>
#include <Collision/Collision.h>

#include <Math/Math.h>

void CollisionCell::AddEntity(entt::entity handle)
{
	// If already in list, then do not add
	for (int i = 0; i < count; ++i)
		if (handle == entities[i])
			return;

	if (count >= MAX_ENTITIES)
		return;

	entities[count++] = handle;
}





void CollisionZone::Init(float widthMax, float heightMax, float cellSize)
{
	// x and y needs to be odd so that center is simple.
	width = static_cast<int>(widthMax / cellSize + 0.5f);
	height = static_cast<int>(heightMax / cellSize + 0.5f);
	
	if (width % 2 == 0)
		++width;
	if (height % 2 == 0)
		++height;

	centerX = static_cast<int>((float)width / 2.0f);
	centerY = static_cast<int>((float)height / 2.0f);

	cellMap.resize(width * height);

	LOG_CORE_INFO("Init: Collision Zone Size: ({},{})", width, height);
}

bool CollisionZone::FindAndDispatchCollisions(Timestep ts, Entity e1, std::vector<Entity>& collidedEntities)
{
	TransformComponent& transform1 = e1.GetTransformComponent();

	CollisionComponent& cc1 = e1.GetComponent<CollisionComponent>();
	glm::vec2 collisionMidPos1 = cc1.collisionBox.OffsetNew(transform1.position).GetMidpoint();
	CollisionBox cb1Offseted = cc1.collisionBox.OffsetNew(transform1.position);

	CollisionPositionMapping mapping = GetCollisionPositionMapping(collisionMidPos1);

	for (int y = -1; y < 2; ++y)
	{
		for (int x = -1; x < 2; ++x)
		{
			CollisionPositionMapping mappingOffset;
			mappingOffset.cellX = mapping.cellX + x;
			mappingOffset.cellY = mapping.cellY + y;
			
			// get cell
			std::optional<CollisionCell*> cellOptional = GetCell(mappingOffset);
			if (!cellOptional.has_value())
				continue;

			CollisionCell* cell = cellOptional.value();

			for (int i = 0; i < cell->count; ++i)
			{
				Entity e2 = scene.WrapEntityHandle(cell->entities[i]);

				if (!e2.IsValid())
					continue;
				if (e1 == e2)
					continue;
				
				CollisionComponent& cc2 = e2.GetComponent<CollisionComponent>();
				CollisionBox cb2Offseted = cc2.collisionBox.OffsetNew(e2.GetTransformComponent().position);

				bool collides = cb1Offseted.CollidesWith(&cb2Offseted);

				// Debug data
				++collisionCheckCount;

				if (collides)
					collidedEntities.push_back(e2);
			}
		}
	}

	return !collidedEntities.empty();
}

CollisionPositionMapping CollisionZone::GetCollisionPositionMapping(const glm::vec2& pos)
{
	int cellX = Math::ffloor((pos.x - playerOffset.x) / cellSize) + centerX;
	int cellY = Math::ffloor((pos.y - playerOffset.y) / cellSize) + centerY;

	return CollisionPositionMapping{ cellX, cellY };
}

std::pair<glm::vec2, Entity> CollisionZone::HeuristicNearest(const glm::vec2& pos, const float maxDistance)
{
	std::pair<glm::vec2, Entity> result{ glm::vec2{0}, {} };

	CollisionPositionMapping positionMapping = GetCollisionPositionMapping(pos);

	return std::pair<glm::vec2, Entity>();
}

std::optional<CollisionCell*> CollisionZone::GetCell(const CollisionPositionMapping& mapping)
{
	if (mapping.cellX >= 0 && mapping.cellX < width &&
		mapping.cellY >= 0 && mapping.cellY < height)
		return &cellMap[mapping.cellY * width + mapping.cellX];

	return std::optional<CollisionCell*>();
}

void CollisionZone::Clear()
{
	int total = 0;
	for (CollisionCell& cell : cellMap)
	{
		total += cell.count;
		cell.count = 0;
	}
	//LOG_CORE_INFO("Total: {}", total);
}




std::string CollisionPositionMapping::ToString() const
{
	return std::format("Cell({},{})", cellX, cellY);
}
