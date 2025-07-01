#include "pch.h"
#include "MovementSystems.h"
#include "HeadersUpdateSystems.h"

void EntitySystem::ResetMovementSystem(Timestep ts, Scene& scene)
{
	// Reset Move Component
	auto view = scene.getRegistry().view<MoveComponent>();

	for (entt::entity e : view)
	{
		auto& mc = view.get<MoveComponent>(e);
		if (!mc.locked) mc.zero();
	}
}

void EntitySystem::MovementSystem(Timestep ts, Scene& scene)
{
	auto view = scene.getRegistry().view<TransformComponent, MoveComponent>();

	for (entt::entity e : view)
	{
		auto [transform, move] = view.get(e);
		transform.position += glm::vec3{ move.moveVec * move.speed * (float)ts, 0.0f };
	}
}

void EntitySystem::JumpSystem(Timestep ts, Scene& scene)
{

	// Jump component
	auto view = scene.getRegistry().view<JumpComponent, VisualComponent>();

	for (entt::entity e : view)
	{
		auto [jc, vc] = view.get<JumpComponent, VisualComponent>(e);

		jc.velocity -= 0.8f * (float)ts;
		//jc.acceleration -= 0.4f * (float)delta;
		jc.z += jc.velocity;
		if (jc.z <= 0)
		{
			jc.z = 0;
			jc.velocity = 0;
			jc.acceleration = 0;
		}

		vc.localTransform.z = jc.z;
	}
}

void EntitySystem::LifeSystem(Timestep ts, Scene& scene)
{

	auto view = scene.getRegistry().view<TimeLifeComponent>();

	for (entt::entity e : view)
	{
		auto [tlc] = view.get(e);
		if (tlc.timeRemaining <= 0.0f)
		{
			Entity entity = scene.WrapEntityHandle(e);
			entity.Destroy();
		}
		else
			tlc.timeRemaining -= ts;
	}
}
