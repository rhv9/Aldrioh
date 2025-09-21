#include "pch.h"
#include "RenderSystems.h"
#include "HeadersRenderSystems.h"
#include <Components/Collision.h>

#include <Debug/GameDebugState.h>

void EntitySystem::EntityRenderSystem(Timestep ts, Scene& scene)
{
	auto view = scene.getRegistry().view<TransformComponent, VisualComponent>();

	for (entt::entity e : view)
	{
		auto [transform, visual] = view.get(e);
		glm::vec3 drawTransform = { transform.position.x + visual.localTransform.x, transform.position.y + visual.localTransform.y, RenderDepth::ENTITY };
		RenderQueue::EnQueue(RenderLayer::ONE, drawTransform + glm::vec3{ 0.0f, -0.4f, 0.0f }, Sprites::shadow, { 1, 1 });
		RenderQueue::EnQueue(RenderLayer::TWO, drawTransform + glm::vec3{ 0.0f, visual.localTransform.z, 0.0f }, visual.spriteId, { 1, 1 });
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
		//RenderQueue::EnQueue(RenderLayer::THREE, offset, Sprites::redBox, cb.size);
	}
}
