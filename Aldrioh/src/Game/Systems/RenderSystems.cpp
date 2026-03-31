#include "pch.h"
#include "RenderSystems.h"
#include <Systems/HeadersRenderSystems.h>
#include <Collision/Collision.h>
#include <Components/Path/PathComponent.h>

#include <Game/Debug/GameDebugState.h>
#include <Game.h>

glm::vec2 EntitySystem::CalculateEntityTransformWithInterpolation(Entity entity, Timestep ts)
{
	glm::vec2 entityTransform{ 0 };
	TransformComponent& tc = entity.GetComponent<TransformComponent>();
	entityTransform.x = tc.position.x;
	entityTransform.y = tc.position.y;

	const glm::vec2& diff = entityTransform - tc.prevPosition;
	entityTransform.x -= diff.x * (1.0f - ts);
	entityTransform.y -= diff.y * (1.0f - ts);

	return entityTransform;
}

void EntitySystem::EntityRenderSystem(Timestep ts, Scene& scene)
{
	auto view = scene.getRegistry().view<TransformComponent, VisualComponent>();

	for (entt::entity e : view)
	{
		auto [transform, visual] = view.get(e);
		Entity entity = scene.WrapEntityHandle(e);

		glm::vec2 entityPos = transform.CalculateInterpolatePosition(ts);

		glm::vec3 drawTransform = { entityPos.x + visual.localTransform.x, entityPos.y + visual.localTransform.y, RenderDepth::ENTITY };

		RenderQueue::EnQueue(visual.renderLayer, drawTransform, visual.spriteId, visual.colour, visual.scale, visual.rotation, visual.flags);
	}
}

void EntitySystem::CollisionRenderSystem(Timestep ts, Scene& scene)
{
	if (!GameDebugState::showCollisionBox)
		return;

	auto view = scene.getRegistry().view<TransformComponent, CollisionComponent>();

	for (entt::entity e : view)
	{
		auto [tc, cc] = view.get<TransformComponent, CollisionComponent>(e);
		glm::vec2 offset = tc.position + cc.collisionBox.position;
		RenderQueue::EnQueue(RenderLayer::THREE, glm::vec3{ offset, 0.0f }, Sprites::borderBox, Colour::RED, cc.collisionBox.size);
	}
}
