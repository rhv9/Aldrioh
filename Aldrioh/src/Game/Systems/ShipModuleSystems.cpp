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
			
			e.GetComponent<PhysicsMovementComponent>().naturalFallOffMultiplier = 0.5f;

			auto& mcc = e.GetComponent<MoveControllerComponent>();
			mcc.moveDir = Math::angleToNormalizedVector(rsc.angleFacing);
		}
		else if (rsc.active && rsc.elapsedTime <= rsc.timer * 10.0f)
		{
		
			
			glm::vec2 mousePos = scene.GetMousePosInScene();
			
			// Rotate towards mouse pos;
			glm::vec2 pos = e.GetTransformComponent().position;
			float angle = Math::angleBetween2d(pos, mousePos);
			float rocketAngle = rsc.angleFacing;
			float angleDiff = angle - rocketAngle;
			float angleTurn = Math::min(Math::abs(angleDiff), rsc.rotationSpeed * ts) * Math::sign(angleDiff);
			LOG_CORE_INFO("Angle diff {}, rocketAngle: {}, angleToMouse: {}", angleDiff, rocketAngle, angle);
			rsc.angleFacing += angleTurn;

			auto& mcc = e.GetComponent<MoveControllerComponent>();
			mcc.moveDir = Math::angleToNormalizedVector(rsc.angleFacing);
			e.GetComponent<VisualComponent>().rotation = rsc.angleFacing + RocketShooterComponent::ROTATION_OFFSET;
		}

	}
}
