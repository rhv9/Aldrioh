#include "pch.h"
#include "RenderSystems.h"
#include <Systems/HeadersRenderSystems.h>
#include <Collision/Collision.h>
#include <Components/Path/PathComponent.h>

#include <Game/Debug/GameDebugState.h>
#include <Game.h>

void EntitySystem::EntityRenderSystem(Timestep ts, Scene& scene)
{
	auto view = scene.getRegistry().view<TransformComponent, VisualComponent>();

	for (entt::entity e : view)
	{
		auto [transform, visual] = view.get(e);
		Entity entity = scene.WrapEntityHandle(e);

		glm::vec2 entityPos = transform.CalculateInterpolatePosition(ts);

		glm::vec3 drawTransform = { entityPos.x, entityPos.y, RenderDepth::ENTITY };
		drawTransform.x += visual.flipX ? -visual.localTransform.x : visual.localTransform.x;
		drawTransform.y += visual.localTransform.y;

		glm::vec2 scale{ visual.flipX ? -visual.scale.x : visual.scale.x, visual.scale.y };

		RenderQueue::EnQueue(visual.renderLayer, drawTransform, visual.spriteId, visual.colour, scale, visual.rotation, visual.flags);
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
		RenderQueue::EnQueue(RenderLayer::FOUR, glm::vec3{ offset, 0.8f }, Sprites::borderBox, Colour::RED, cc.collisionBox.size);
	}
}
