#include "pch.h"
#include "CollisionSystems.h"
#include <Systems/HeadersUpdateSystems.h>
#include <Systems/HeadersRenderSystems.h>
#include <Collision/Collision.h>
#include <Game/Components/LevelComponents.h>

#include <Math/Math.h>

#include <Game/Debug/GameDebugState.h>

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
}

void EntitySystem::ResetAndAddCollisionWorld(Timestep ts, Scene& scene)
{
	CollisionWorld& collisionWorld = scene.GetCollisionWorld();
	BoundingArea deathArea = scene.GetFirstEntity<LevelComponent>().GetComponent<LevelComponent>().level->GetDeathArea();
	glm::vec2 maxActualPos = collisionWorld.GetMaxActualPosition();

	// Debug
	collisionWorld.ResetCollisionCheckCount();
	// Reset 
	{
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
		auto view = scene.getRegistry().view<TransformComponent, CollisionComponent>();


		for (auto eHandle : view)
		{
			auto [tc, cb] = view.get<TransformComponent, CollisionComponent>(eHandle);


			// Get collision centre pos and use that.
			glm::vec2 midPos = cb.collisionBox.OffsetNew(tc.position).GetMidpoint();

			// if within Death area, then do things with collision
			if (midPos.x < deathArea.bottomLeft.x || midPos.y < deathArea.bottomLeft.y ||
				midPos.x > deathArea.topRight.x || midPos.y > deathArea.topRight.y)
				continue;

			if (midPos.x < 0.0f || midPos.y < 0.0f || midPos.x >= maxActualPos.x || midPos.y >= maxActualPos.y)
				continue;

			CollisionWorld& collisionWorld = scene.GetCollisionWorld();
			PositionMapping positionMapping = collisionWorld.GetMapping(midPos);


			Cell& cell = collisionWorld.GetChunk(positionMapping.chunkX, positionMapping.chunkY).GetCell(positionMapping.cellX, positionMapping.cellY);

			cell.AddEntity(eHandle);

		}
	}
}

void EntitySystem::DebugRenderCollisionWorldVisualisation(Timestep ts, Scene& scene)
{
	if (GameDebugState::showCollisionWorldVisualisation)
	{
		CollisionWorld& collisionWorld = scene.GetCollisionWorld();
		Level* level = scene.GetFirstComponent<LevelComponent>().level;
		BoundingArea deathArea = level->GetDeathArea();
		glm::vec2 playerCameraPos = level->GetPlayerCamera().GetComponent<CameraComponent>().cameraController->GetPosition();

		// Rendering jewels in the screen
		int startX = static_cast<int>(deathArea.bottomLeft.x);
		int startY = static_cast<int>(deathArea.bottomLeft.y);
		int endX = static_cast<int>(deathArea.topRight.x);
		int endY = static_cast<int>(deathArea.topRight.y);

		for (int y = startY; y < endY; ++y)
		{
			for (int x = startX; x < endX; ++x)
			{
				RenderQueue::EnQueue(RenderLayer::FOUR, { x, y, RenderDepth::COLLECTABLES }, Sprites::borderBox, Colour::GREEN);
			}
		}
	}


}
