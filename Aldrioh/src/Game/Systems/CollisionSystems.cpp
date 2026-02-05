#include "pch.h"
#include "CollisionSystems.h"
#include <Systems/HeadersUpdateSystems.h>
#include <Systems/HeadersRenderSystems.h>
#include <Collision/Collision.h>
#include <Game/Components/LevelComponents.h>

#include <Math/Math.h>

#include <Game/Debug/GameDebugState.h>

#include <UI/Font.h>
#include <Graphics/SubTexture.h>

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

		int startX = static_cast<int>(collisionZoneOffset.x) - collisionZone.GetCenterXCellPos();
		int startY = static_cast<int>(collisionZoneOffset.y) - collisionZone.GetCenterYCellPos();
		int endX = startX + collisionZone.GetWidth();
		int endY = startY + collisionZone.GetHeight();

		glm::vec2 cellSize{ collisionZone.GetCellSize(), collisionZone.GetCellSize() };

		for (int y = startY; y < endY; ++y)
		{
			for (int x = startX; x < endX; ++x)
			{
				// Render collision Cells
				RenderQueue::EnQueue(RenderLayer::FOUR, { x, y, RenderDepth::COLLECTABLES }, Sprites::borderBox, Colour::BLUE, cellSize);

				static FontStyle fontStyle = FontStyle{}.WithSize(0.4f).WithColour({ 0.8f,0.8f, 0.8f, 1.0f });

				std::optional<CollisionCell*> cellOptional = collisionZone.GetCell(collisionZone.GetCollisionPositionMapping({ x, y }));
				CollisionCell* cell = cellOptional.value();
				std::string text = std::to_string(cell->count);
				float numWidth = fontStyle.CalculateTextWidth(text);
				RenderQueue::EnQueueText(RenderLayer::FOUR, { x + 0.5f - numWidth / 2, y + 0.5f - fontStyle.size / 2, RenderDepth::COLLECTABLES }, &fontStyle, text, Colour::WHITE, { fontStyle.size, fontStyle.size });

			}
		}
	}
}
