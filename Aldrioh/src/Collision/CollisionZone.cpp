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

	if (count >= 16)
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

bool CollisionZone::FindAndDispatchCollisions(Timestep ts, Entity e1, CollisionDispatcher& dispatcher)
{
	bool hasCollided = false;
	TransformComponent& transform1 = e1.GetTransformComponent();
	MoveComponent& move1 = e1.GetComponent<MoveComponent>();

	glm::vec2 movedPos1 = transform1.position + move1.CalculateActualMoveOffsetVec3(ts);;

	CollisionComponent& cc1 = e1.GetComponent<CollisionComponent>();
	glm::vec2 collisionMidPos1 = cc1.collisionBox.OffsetNew({ movedPos1, 0.0f }).GetMidpoint();
	CollisionBox cb1Offseted = cc1.collisionBox.OffsetNew(glm::vec3{ movedPos1, 0.0f });

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
				if (e1.HasComponent<CollisionHandledComponent>())
					return true;
				if (e2.HasComponent<CollisionHandledComponent>())
					continue;

				TransformComponent& transform2 = e2.GetTransformComponent();
				MoveComponent& move2 = e2.GetComponent<MoveComponent>();
				glm::vec3 pos2 = transform2.position;
				CollisionComponent& cc2 = e2.GetComponent<CollisionComponent>();

				CollisionBox cb2Offseted = cc2.collisionBox.OffsetNew(pos2);
				bool collides = cb1Offseted.CollidesWith(&cb2Offseted);

				// Debug data
				++collisionCheckCount;

				if (collides)
				{
					hasCollided = true;

					// if both are rigid objects, then do push out effect
					if (cc1.rigid && cc2.rigid)
					{
						float pushout = 0.30f;
						glm::vec2 direction = cb1Offseted.GetMidpoint() - cb2Offseted.GetMidpoint();
						glm::vec2 normalizedDirection = glm::normalize(direction);

						move1.addMoveVec({ normalizedDirection.x ,normalizedDirection.y }, pushout);
						move2.addMoveVec({ normalizedDirection.x, normalizedDirection.y }, -pushout);

						movedPos1 = transform1.position + move1.CalculateActualMoveOffsetVec3(ts);;

						collisionMidPos1 = cc1.collisionBox.OffsetNew({ movedPos1, 0.0f }).GetMidpoint();
						cb1Offseted = cc1.collisionBox.OffsetNew(glm::vec3{ movedPos1, 0.0f });
					}

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

CollisionPositionMapping CollisionZone::GetCollisionPositionMapping(const glm::vec2& pos)
{
	int cellX = static_cast<int>((pos.x - playerOffset.x) / cellSize) + centerX;
	int cellY = static_cast<int>((pos.y - playerOffset.y) / cellSize) + centerY;

	return CollisionPositionMapping{ cellX, cellY };
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

	LOG_CORE_INFO("Total: {}", total);
}




std::string CollisionPositionMapping::ToString() const
{
	return std::format("Cell({},{})", cellX, cellY);
}
