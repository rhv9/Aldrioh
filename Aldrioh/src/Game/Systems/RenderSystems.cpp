#include "pch.h"
#include "RenderSystems.h"
#include <Scene/Scene.h>
#include <Scene/Entity.h>
#include <Scene/Components.h>
#include <Game/RenderDepth.h>
#include <Game/SpriteCollection.h>

#include <Graphics/Renderer.h>
#include <Components/Collision.h>

void EntitySystem::EntityRenderSystem(Timestep ts, Scene& scene)
{
	auto view = scene.getRegistry().view<TransformComponent, VisualComponent>();

	for (entt::entity e : view)
	{
		auto [transform, visual] = view.get(e);
		glm::vec3 drawTransform = { transform.position.x + visual.localTransform.x, transform.position.y + visual.localTransform.y, RenderDepth::ENTITY };
		Renderer::DrawQuad(drawTransform + glm::vec3{ 0.0f, -0.4f, 0.0f }, Sprites::get(Sprites::shadow), { 1, 1 });
		Renderer::DrawQuad(drawTransform + glm::vec3{ 0.0f, visual.localTransform.z, 0.0f }, Sprites::get(visual.spriteId), { 1, 1 });
	}
}

void EntitySystem::CollisionRenderSystem(Timestep ts, Scene& scene)
{
	auto view = scene.getRegistry().view<TransformComponent, CollisionBox>();

	for (entt::entity e : view)
	{
		auto [tc, cb] = view.get<TransformComponent, CollisionBox>(e);
		glm::vec3 offset = tc.position + cb.position;
		Renderer::DrawQuad(offset, Sprites::get(Sprites::redBox), cb.size);
	}
}
