#include "pch.h"
#include "RenderSystems.h"
#include <Systems/HeadersRenderSystems.h>
#include <Components/Collision.h>

#include <Debug/GameDebugState.h>
#include <Game.h>

void EntitySystem::EntityRenderSystem(Timestep ts, Scene& scene)
{
	auto view = scene.getRegistry().view<TransformComponent, VisualComponent>();

	for (entt::entity e : view)
	{
		auto [transform, visual] = view.get(e);
		Entity entity = scene.WrapEntityHandle(e);
		
		glm::vec3 drawTransform = { transform.position.x + visual.localTransform.x, transform.position.y + visual.localTransform.y, RenderDepth::ENTITY };

		if (entity.HasComponent<MoveComponent>())
		{
			MoveComponent& mc = entity.GetComponent<MoveComponent>();
			Timestep delta = Game::Instance().GetFixedUpdateTimestep();
			drawTransform.x -= mc.moveVec.x * mc.speed * delta * (1.0f-ts);
			drawTransform.y -= mc.moveVec.y * mc.speed * delta * (1.0f-ts);

		}

		RenderQueue::EnQueue(RenderLayer::TWO, drawTransform + glm::vec3{ 0.0f, visual.localTransform.z, 0.0f }, visual.spriteId, visual.colour, visual.scale, visual.rotation, visual.flags);
	}
}

void EntitySystem::CollisionRenderSystem(Timestep ts, Scene& scene)
{
	if (!GameDebugState::showCollisionBox)
		return;

	//auto view = scene.getRegistry().view<TransformComponent, CollisionBox>();
	//
	//for (entt::entity e : view)
	//{
	//	auto [tc, cb] = view.get<TransformComponent, CollisionBox>(e);
	//	glm::vec3 offset = tc.position + cb.position;
	//	RenderQueue::EnQueue(RenderLayer::THREE, offset, Sprites::redBox, cb.size);
	//}
}
