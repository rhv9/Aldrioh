#include "pch.h"
#include "RenderSystems.h"
#include <Systems/HeadersRenderSystems.h>
#include <Components/Collision.h>
#include <Components/Path/PathComponent.h>

#include <Debug/GameDebugState.h>
#include <Game.h>

glm::vec2 EntitySystem::CalculateEntityTransformWithInterpolation(Entity entity, Timestep ts)
{
	glm::vec2 entityTransform{ 0 };
	TransformComponent& tc = entity.GetComponent<TransformComponent>();
	entityTransform.x = tc.position.x;
	entityTransform.y = tc.position.y;

	if (entity.HasComponent<MoveComponent>())
	{
		MoveComponent& mc = entity.GetComponent<MoveComponent>();
		Timestep delta = Game::Instance().GetFixedUpdateTimestep();
		entityTransform.x -= mc.moveVec.x * mc.speed * delta * (1.0f - ts);
		entityTransform.y -= mc.moveVec.y * mc.speed * delta * (1.0f - ts);

	}

	if (entity.HasComponent<PathComponent>())
	{
		PathComponent& pc = entity.GetComponent<PathComponent>();
		Timestep delta = Game::Instance().GetFixedUpdateTimestep();
		const glm::vec2& diff = pc.currentPosition - pc.prevPosition;
		entityTransform.x -= diff.x * (1.0f - ts);
		entityTransform.y -= diff.y * (1.0f - ts);
	}

	return entityTransform;
}

void EntitySystem::EntityRenderSystem(Timestep ts, Scene& scene)
{
	auto view = scene.getRegistry().view<TransformComponent, VisualComponent>();

	for (entt::entity e : view)
	{
		auto [transform, visual] = view.get(e);
		Entity entity = scene.WrapEntityHandle(e);
		
		glm::vec2 entityPos = CalculateEntityTransformWithInterpolation(entity, ts);

		glm::vec3 drawTransform = { entityPos.x + visual.localTransform.x, entityPos.y + visual.localTransform.y, RenderDepth::ENTITY };



		RenderQueue::EnQueue(visual.renderLayer, drawTransform, visual.spriteId, visual.colour, visual.scale, visual.rotation, visual.flags);
	}
}

void EntitySystem::CollisionRenderSystem(Timestep ts, Scene& scene)
{
	if (!GameDebugState::showCollisionBox)
		return;

	auto view = scene.getRegistry().view<TransformComponent, CollisionBox>();
	
	for (entt::entity e : view)
	{
		auto [tc, cb] = view.get<TransformComponent, CollisionBox>(e);
		glm::vec3 offset = tc.position + cb.position;
		RenderQueue::EnQueue(RenderLayer::THREE, offset, Sprites::redBox, Colour::RED, cb.size);
	}
}
