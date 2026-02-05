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

void EntitySystem::ResetAndAddCollisionZone(Timestep ts, Scene& scene)
{
	// Reset
	scene.GetCollisionZone().Clear();
	scene.GetCollisionZone().ResetCollisionCheckCount();

	scene.GetCollisionZone().SetPlayerOffset(scene.GetPlayer().GetTransformComponent().position);
	BoundingArea deathArea = scene.GetFirstEntity<LevelComponent>().GetComponent<LevelComponent>().level->GetDeathArea();

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

			CollisionZone& collisionZone = scene.GetCollisionZone();
			CollisionPositionMapping positionMapping = collisionZone.GetCollisionPositionMapping(midPos);

			std::optional<CollisionCell*> cellOptional = collisionZone.GetCell(positionMapping);
			if (!cellOptional.has_value())
				continue;

			CollisionCell* cell = cellOptional.value();
			cell->AddEntity(eHandle);

		}
	}
}

void EntitySystem::DebugRenderCollisionZoneVisualisation(Timestep ts, Scene& scene)
{
	if (GameDebugState::showCollisionZoneVisualisation)
	{


		CollisionZone& collisionZone = scene.GetCollisionZone();
		Level* level = scene.GetFirstComponent<LevelComponent>().level;
		BoundingArea deathArea = level->GetDeathArea();
		glm::vec2 playerCameraPos = level->GetPlayerCamera().GetComponent<CameraComponent>().cameraController->GetPosition();
		glm::vec2 collisionZoneOffset = collisionZone.GetPlayerOffset();

		int startX = static_cast<int>(collisionZoneOffset.x);
		int startY = static_cast<int>(collisionZoneOffset.y);
		int endX = static_cast<int>(deathArea.topRight.x);
		int endY = static_cast<int>(deathArea.topRight.y);



		// Render collision Cells
		RenderQueue::EnQueue(RenderLayer::FOUR, { startX, startY, RenderDepth::COLLECTABLES }, Sprites::borderBox, Colour::BLUE);
	}


}
