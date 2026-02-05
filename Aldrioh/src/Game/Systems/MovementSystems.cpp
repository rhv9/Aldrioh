#include "pch.h"
#include "MovementSystems.h"
#include <Game/Components/EntityComponents.h>
#include <Systems/HeadersUpdateSystems.h>

#include <Collision/CollisionDispatcher.h>
#include <Collision/CollisionWorld.h>
#include <Collision/Collision.h>

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
		Entity entityWrapped = scene.WrapEntityHandle(e);
		if (entityWrapped.HasComponent<CollisionComponent>())
		{
			scene.DispatchCollisions(ts, entityWrapped);

		}
	}

	for (entt::entity e : view)
	{
		auto [transform, move] = view.get(e);
		transform.position += move.CalculateActualMoveOffsetVec3(ts);
	}

	// Remove all handled collision components
	{
		auto view = scene.getRegistry().view<CollisionHandledComponent>();

		for (auto e : view)
			scene.WrapEntityHandle(e).RemoveComponent<CollisionHandledComponent>();
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
			entity.QueueDestroy();
		}
		else
			tlc.timeRemaining -= ts;
	}
}
