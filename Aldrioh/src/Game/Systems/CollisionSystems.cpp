#include "pch.h"
#include "CollisionSystems.h"
#include <Systems/HeadersUpdateSystems.h>
#include <Collision/Collision.h>
#include <Game/Components/LevelComponents.h>

#include <Math/Math.h>

void EntitySystem::CollisionSystem(Timestep ts, Scene& scene)
{
	{
		auto view = scene.getRegistry().view<TransformComponent, CollisionBox>();

		for (auto iter = view.begin(); iter != view.end(); ++iter)
		{

			auto copiedIter = iter;
			copiedIter++;
			for (auto innerIter = copiedIter; innerIter != view.end(); ++innerIter)
			{
				entt::entity e1 = *iter;
				entt::entity e2 = *innerIter;
				Entity entity1 = scene.WrapEntityHandle(e1);
				Entity entity2 = scene.WrapEntityHandle(e2);

				if (entity1.HasComponent<CollisionHandledComponent>())
					break;
				if (entity2.HasComponent<CollisionHandledComponent>())
					continue;

				auto [tc1, cb1] = view.get(e1);
				auto [tc2, cb2] = view.get(e2);

				CollisionBox cb2Collision = cb2.OffsetNew(tc2.position);
				bool collides = cb1.OffsetNew(tc1.position).CollidesWith(&cb2Collision);

				if (collides)
				{
					CollisionEvent eventEntity1{ entity1, false };
					CollisionEvent eventEntity2{ entity2, false };
					scene.GetCollisionDispatcher().Dispatch(eventEntity1, eventEntity2);

					if (eventEntity1.handled)
						entity1.AddComponent<CollisionHandledComponent>();
					if (eventEntity2.handled)
						entity2.AddComponent<CollisionHandledComponent>();
				}

			}
		}
	}

	// Remove all handled collision components
	{
		auto view = scene.getRegistry().view<CollisionHandledComponent>();

		for (auto e : view)
			scene.WrapEntityHandle(e).RemoveComponent<CollisionHandledComponent>();
	}




}

void EntitySystem::ResetAndAddCollisionWorld(Timestep ts, Scene& scene)
{
	// Reset 
	{
		CollisionWorld& collisionWorld = scene.GetCollisionWorld();
		BoundingArea deathArea = scene.GetFirstEntity<LevelComponent>().GetComponent<LevelComponent>().level->GetDeathArea();

		glm::vec2 maxActualPos = collisionWorld.GetMaxActualPosition();
		
		glm::vec2 bottomLeft, topRight;
		bottomLeft.x = Math::minAndMax(0, deathArea.bottomLeft.x, maxActualPos.x);
		bottomLeft.y = Math::minAndMax(0, deathArea.bottomLeft.y, maxActualPos.y);
		topRight.x = Math::minAndMax(0, deathArea.topRight.x, maxActualPos.x);
		topRight.y = Math::minAndMax(0, deathArea.topRight.y, maxActualPos.y);

		PositionMapping bottomLeftMapping = collisionWorld.GetMapping(bottomLeft);
		PositionMapping topRightMapping = collisionWorld.GetMapping(topRight);
		
		int startX = bottomLeftMapping.chunkX;
		int startY = bottomLeftMapping.chunkY;
		int endX = topRightMapping.chunkX;
		int endY = topRightMapping.chunkY;

		//LOG_CORE_INFO("bottomLeft: {}, topRight: {}", glm::to_string(bottomLeft), //glm::to_string(topRight));
		//LOG_CORE_INFO("start: ({},{}), end: ({},{})", startX, startY, endX, endY);
		//LOG_CORE_INFO("Bottom left mapping: {}", bottomLeftMapping.ToString());
		//LOG_CORE_INFO("Top right mapping: {}", topRightMapping.ToString());

		for (int y = startY; y < endY; ++y)
		{
			for (int x = startX; x < endX; ++x)
			{
				collisionWorld.GetChunk(x, y).Clear();
			}
		}
	}

	// Add collisions
	
	{
		auto view = scene.getRegistry().view<TransformComponent, CollisionBox>();

		for (auto eHandle : view)
		{
			auto [tc, cb]= view.get<TransformComponent, CollisionBox>(eHandle);
			
			// Get collision centre pos and use that.
			CollisionBox offsetBox = cb.OffsetNew(tc.position);
			glm::vec2 midPos{ offsetBox.position.x + offsetBox.size.x / 2.0f, offsetBox.position.y + offsetBox.size.y / 2.0f };

			CollisionWorld& collisionWorld = scene.GetCollisionWorld();
			PositionMapping positionMapping = collisionWorld.GetMapping(midPos);

			Cell& cell = collisionWorld.GetChunk(positionMapping.chunkX, positionMapping.chunkY).GetCell(positionMapping.cellX, positionMapping.cellY);

			cell.AddEntity(eHandle);

		}
	}
}
