#include <pch.h>
#include "ShipModuleSystems.h"
#include <Systems/HeadersUpdateSystems.h>
#include <Game/Components/ControllerComponents.h>
#include <Game/SpriteCollection.h>

#include <Game/Level/CollectableManager.h>
#include <Collision/Collision.h>
#include <Game/Entity/EntityType.h>

#include <Game/Components/EntityComponents.h>
#include <Game/Components/LevelComponents.h>
#include <Game/Components/ShipModuleComponents.h>

#include <Math/Math.h>

#include <Game/GlobalLayers.h>
#include <Game/Entity/GameEntities.h>

void EntitySystem::ShipModuleSystems(Timestep ts, Scene& scene)
{
	auto view = scene.getRegistry().view<RocketShooterComponent>();

	for (entt::entity entityHandle : view)
	{
		auto& rsc = view.get<RocketShooterComponent>(entityHandle);
		Entity e = scene.WrapEntityHandle(entityHandle);

		rsc.elapsedTime += ts;

		if (!rsc.active && rsc.elapsedTime >= rsc.timer)
		{
			rsc.active = true;
			e.RemoveComponent<RotationComponent>();
			rsc.angleFacing = e.GetComponent<VisualComponent>().rotation - RocketShooterComponent::ROTATION_OFFSET;
			e.GetComponent<PhysicsMovementComponent>().naturalFallOffMultiplier = 8.0f;

			auto& mcc = e.GetComponent<MoveControllerComponent>();
			mcc.moveDir = Math::angleToNormalizedVector(rsc.angleFacing);

			glm::vec2 mousePos = scene.GetMousePosInScene();
			glm::vec2 pos = e.GetTransformComponent().position;
			rsc.targetAngle = Math::angleBetween2d(pos, mousePos);
		}
		else if (rsc.active && rsc.elapsedTime <= rsc.timer + 1.2f)
		{
			glm::vec2 mousePos = scene.GetMousePosInScene();
			glm::vec2 pos = e.GetTransformComponent().position;
			rsc.targetAngle = Math::angleBetween2d(pos, mousePos);
			float diff = Math::normalizeAngle(rsc.targetAngle - rsc.angleFacing);

			float sign = Math::sign(diff);
			float angleTurn = Math::min(diff * sign, rsc.rotationSpeed * ts) * sign;
			rsc.angleFacing += angleTurn;

			auto& mcc = e.GetComponent<MoveControllerComponent>();
			mcc.moveDir = Math::angleToNormalizedVector(rsc.angleFacing);
			e.GetComponent<VisualComponent>().rotation = rsc.angleFacing + RocketShooterComponent::ROTATION_OFFSET;
		}

	}
}
