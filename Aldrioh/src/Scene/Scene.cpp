#include "pch.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"
#include "Math/Math.h"

#include <Input/Input.h>
#include <Game.h>

#include <Components/Collision.h>
#include <Components/CollisionDispatcher.h>

#include <Graphics/Renderer.h>
#include <Game/SpriteCollection.h>
#include <Core/Platform.h>
#include "Game/RenderDepth.h"

#include <Game/Systems/PlayerControllerSystem.h>

Scene::Scene()
{
	LOG_INFO("Sprite ID for this is {}", Sprites::target);
	width = 14;
	height = 200;
	world = new int[width * height];

	for (int i = 0; i < width * height; i++)
	{
		world[i] = Sprites::sand_1;
	}
}

Scene::~Scene()
{
	delete player;
}

Entity Scene::CreateEntity(const std::string& name)
{
	Entity e = { registry.create(), this };
	e.AddComponent<NameComponent>(name);
	e.AddComponent<TransformComponent>();
	return e;
}

void Scene::SetPlayer(const Entity& e)
{
	if (!player) delete player;
	player = new Entity(e);
}

void Scene::OnUpdate(Timestep ts)
{
	// Reset Move Component
	{
		auto view = registry.view<MoveComponent>();

		for (entt::entity e : view)
			view.get<MoveComponent>(e).zero();
	}

	SystemPlayerController(ts, *player);

	// Jump component
	{
		auto view = registry.view<JumpComponent, VisualComponent>();

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


	{
		auto view = registry.view<TimeLifeComponent>();

		for (entt::entity e : view)
		{
			auto [tlc] = view.get(e);
			if (tlc.timeRemaining <= 0.0f)
				registry.destroy(e);
			else
				tlc.timeRemaining -= ts;
		}
	}

	// AnimatedMovementComponent
	{
		auto view = registry.view<AnimatedMovementComponent, MoveComponent, VisualComponent>();

		for (entt::entity e : view)
		{
			auto [amc, mc, vc] = view.get(e);

			if (mc.dir == MoveDir::NONE)
			{
				amc.reset();
				vc.spriteId = amc.getCurrentSprite();
			}
			else
			{
				if (amc.currentDir != mc.dir)
				{
					amc.currentDir = mc.dir;
					amc.reset();
				}
				amc.update(ts);
				vc.spriteId = amc.getCurrentSprite();

			}
		}
	}

	// DumbAIComponent
	{
		auto view = registry.view<DumbAIComponent, TransformComponent, MoveComponent>();
		auto& player_mc = player->GetComponent<TransformComponent>();
		for (entt::entity e : view)
		{
			auto [tc, mc] = view.get<TransformComponent, MoveComponent>(e);

			glm::vec2 diff{ -tc.position.x + player_mc.position.x, -tc.position.y + player_mc.position.y };
			diff = glm::normalize(diff);

			mc.updateMoveVec(diff);
		}
	}

	// Update positions based on move component
	{
		auto view = registry.view<TransformComponent, MoveComponent>();

		for (entt::entity e : view)
		{
			auto [transform, move] = view.get(e);
			transform.position += glm::vec3{ move.moveVec * move.speed * (float)ts, 0.0f };
		}
	}

	// Check collisions
	{
		auto view = registry.view<TransformComponent, CollisionBox>();

		for (auto iter = view.begin(); iter != view.end(); ++iter)
		{
			auto copiedIter = iter;
			copiedIter++;
			for (auto innerIter = copiedIter; innerIter != view.end(); ++innerIter)
			{
				entt::entity e1 = *iter;
				entt::entity e2 = *innerIter;

				auto [tc1, cb1] = view.get(e1);
				auto [tc2, cb2] = view.get(e2);

				CollisionBox cb2Collision = cb2.OffsetNew(tc2.position);
				bool collides = cb1.OffsetNew(tc1.position).CollidesWith(&cb2Collision);

				if (collides)
				{
					collisionDispatcher.Dispatch(registry, e1, e2);
				}

			}

		}
	}
	auto cameraEntity = GetPrimaryCameraEntity();
	cameraEntity.GetComponent<CameraComponent>().cameraController->OnUpdate(ts);

}

void Scene::OnRender(Timestep ts)
{
	auto& cameraController = GetPrimaryCameraEntity().GetComponent<CameraComponent>().cameraController;
	Renderer::StartScene(cameraController->GetCamera().GetViewProjection());

	auto& bounds = cameraController->GetBounds();
	const glm::vec2& cameraPos = cameraController->GetPosition();
	int startX = (int)std::max(-bounds.Right + cameraPos.x, 0.0f);
	int startY = (int)std::max(-bounds.Top + cameraPos.y, 0.0f);
	int endX = (int)std::min(bounds.Right + cameraPos.x + 1, (float)width);
	int endY = (int)std::min(bounds.Top + cameraPos.y + 1, (float)height);

	for (int y = startY; y < endY; y++)
	{
		for (int x = startX; x < endX; x++)
		{
			int tile = world[y * width + x];
			glm::vec3 renderPos = { x * 1.0f, y * 1.0f, RenderDepth::TILE };
			Renderer::DrawQuad(renderPos, Sprites::get(tile), { 1, 1 });
		}
	}

	{
		glm::vec2 mousePos = Input::GetMousePosition();
		glm::vec2 cameraPos = cameraController->GetPosition();

		Renderer::DrawQuad({ 0, 0, 0.5f }, Sprites::get(Sprites::animBossUp[((int)Platform::GetElapsedTime()) % 4]));
		Renderer::DrawQuad({ 1, 0, 0.5f }, Sprites::get(Sprites::animBossDown[((int)Platform::GetElapsedTime()) % 4]));
		Renderer::DrawQuad({ 2, 0, 0.5f }, Sprites::get(Sprites::animBossLeft[((int)Platform::GetElapsedTime()) % 4]));
		Renderer::DrawQuad({ 3, 0, 0.5f }, Sprites::get(Sprites::animBossRight[((int)Platform::GetElapsedTime()) % 4]));
	}


	// entities
	{
		auto view = registry.view<TransformComponent, VisualComponent>();

		for (entt::entity e : view)
		{
			auto [transform, visual] = view.get(e);
			glm::vec3 drawTransform = { transform.position.x + visual.localTransform.x, transform.position.y + visual.localTransform.y, RenderDepth::ENTITY };
			Renderer::DrawQuad(drawTransform + glm::vec3{ 0.0f, -0.4f, 0.0f }, Sprites::get(Sprites::shadow), { 1, 1 });
			Renderer::DrawQuad(drawTransform + glm::vec3{ 0.0f, visual.localTransform.z, 0.0f }, Sprites::get(visual.spriteId), { 1, 1 });
		}
	}

	// Render collision
	{
		auto view = registry.view<TransformComponent, CollisionBox>();

		for (entt::entity e : view)
		{
			auto [tc, cb] = view.get<TransformComponent, CollisionBox>(e);
			glm::vec3 offset = tc.position + cb.position;
			Renderer::DrawQuad(offset, Sprites::get(Sprites::redBox), cb.size);
		}
	}



	Renderer::EndScene();
}

Entity Scene::WrapEntityHandle(entt::entity entityHandle)
{
	return { entityHandle, this };
}

Entity Scene::GetPrimaryCameraEntity()
{
	auto view = registry.view<CameraComponent>();

	// TODO: Currently accepts only one camera as main camera, update to support more
	for (entt::entity e : view)
	{
		return Entity(e, this);
	}

	LOG_CORE_CRITICAL("No camera added to scene!");

	return Entity();
}

glm::vec2 Scene::GetMousePosInScene()
{
	glm::vec2 mousePos = Input::GetMousePosition();
	auto& cameraComponent = GetPrimaryCameraEntity().GetComponent<CameraComponent>();
	glm::vec2 cameraPos = cameraComponent.cameraController->GetPosition();
	auto& bounds = cameraComponent.cameraController->GetBounds();

	glm::vec2 mousePosPercent = { mousePos.x / Game::Instance().GetWindow()->GetWidth(),
						 mousePos.y / Game::Instance().GetWindow()->GetHeight() };


	glm::vec2 zoomedDimensions{ bounds.Right, -bounds.Top };

	return (glm::vec2{ mousePosPercent.x * zoomedDimensions.x - bounds.Right / 2.0f, mousePosPercent.y * zoomedDimensions.y + bounds.Top / 2.0f } *2.0f) + cameraComponent.cameraController->GetPosition();
}

CollisionDispatcher& Scene::GetCollisionDispatcher()
{
	return collisionDispatcher;
}

