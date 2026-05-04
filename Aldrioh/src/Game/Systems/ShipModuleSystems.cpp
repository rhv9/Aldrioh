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


ParticleTemplate particleTemplate_rocketActive = []() {
	ParticleTemplate pt;
	pt.beginColour = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);
	pt.endColour = glm::vec4(1.0f, 0.5f, 0.0f, 0.9f);
	pt.beginSize = (1.0f / 16.0f) * 2.0f;
	pt.endSize = (1.0f / 16.0f) * 2.0f;
	pt.life = 0.2f;
	pt.velocity = { 0.0f, 0.0f };
	pt.velocityVariation = { 2.0f, 0.6f };
	pt.rotationRange = { Math::degreesToRad(-45), Math::degreesToRad(45) };
	pt.count = 4;
	return pt;
	}();

ParticleTemplate particleTemplate_rocketExhausted = []() {
	ParticleTemplate pt;
	pt.beginColour = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
	pt.endColour = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	pt.beginSize = (1.0f / 16.0f) * 3.0f;
	pt.endSize = (1.0f / 16.0f) * 2.0f;
	pt.life = 1.0f;
	pt.velocity = { 0.0f, 0.0f };
	pt.velocityVariation = { 1.0f, 1.0f };
	pt.rotationRange = { Math::degreesToRad(-45), Math::degreesToRad(45) };
	pt.count = 10;
	return pt;
	}();


void EntitySystem::ShipModuleSystems(Timestep ts, Scene& scene)
{
	auto view = scene.getRegistry().view<RocketShooterComponent>();

	for (entt::entity entityHandle : view)
	{
		auto& rsc = view.get<RocketShooterComponent>(entityHandle);
		Entity e = scene.WrapEntityHandle(entityHandle);

		rsc.elapsedTime += ts;

		if (rsc.state == RocketShooterComponent::State::NOT_ACTIVE && rsc.elapsedTime >= rsc.timer)
		{
			rsc.state = RocketShooterComponent::State::ACTIVE;
			e.RemoveComponent<RotationComponent>();
			rsc.angleFacing = e.GetComponent<VisualComponent>().rotation - RocketShooterComponent::ROTATION_OFFSET;
			e.GetComponent<PhysicsMovementComponent>().naturalFallOffMultiplier = 8.0f;

			auto& mcc = e.GetComponent<MoveControllerComponent>();
			mcc.moveDir = Math::angleToNormalizedVector(rsc.angleFacing);

			glm::vec2 mousePos = scene.GetMousePosInScene();
			glm::vec2 pos = e.GetTransformComponent().position;
			rsc.targetAngle = Math::angleBetween2d(pos, mousePos);
		}
		else if (rsc.state == RocketShooterComponent::State::ACTIVE && rsc.elapsedTime <= rsc.timer + 1.2f)
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

			particleTemplate_rocketActive.startPos = pos - mcc.moveDir / 2.2f;
			particleTemplate_rocketActive.velocity = -mcc.moveDir * 4.0f;
			scene.GetParticleManager().Emit(particleTemplate_rocketActive);
		}
		else if (rsc.state == RocketShooterComponent::State::ACTIVE)
		{
			rsc.state = RocketShooterComponent::State::EXHAUSTED;
			// emit final particle

			// Switch to fixed movement
			auto& mcc = e.GetComponent<MoveControllerComponent>();
			mcc.moveDir = glm::vec2{ 0 };
			
			auto& pmc = e.GetComponent<PhysicsMovementComponent>();
			pmc.resultantVelocity = mcc.moveDir * mcc.speed;
			pmc.managedVelocity = glm::vec2{ 0.0f };
			pmc.naturalFallOffMultiplier = 0.0f;

			glm::vec2 pos = e.GetTransformComponent().position;
			particleTemplate_rocketExhausted.startPos = pos + mcc.moveDir * mcc.speed * (float)ts;
			particleTemplate_rocketExhausted.velocity = -mcc.moveDir * 4.0f;
			scene.GetParticleManager().Emit(particleTemplate_rocketExhausted);
		}

	}
}
