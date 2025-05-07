#include "pch.h"
#include "Level.h"

#include "Game/SpriteCollection.h"
#include "Graphics/Renderer.h"

#include "Math/Math.h"
#include <Scene/Components.h>
#include <Scene/FreeRoamEntityCameraController.h>
#include <Input/Input.h>
#include <Game.h>
#include <imgui.h>
#include "Game/RenderDepth.h"

#include "Core/Platform.h"

Level::Level()
{
	LOG_INFO("Sprite ID for this is {}", Sprites::target);
	width = 14;
	height = 200;
	world = new int[width * height];

	for (int i = 0; i < width * height; i++)
	{
		world[i] = Sprites::sand_1;
	}


	// Create player
	player = registry.create();
	TransformComponent& tc = registry.emplace<TransformComponent>(player);
	tc.position.z = 0.4f;
	tc.position.x = 0.0f;
	tc.position.y = 0.0f;
	VisualComponent& pvc = registry.emplace<VisualComponent>(player, Sprites::player_head);
	pvc.localTransform = { -0.5f, -0.5f, 0.0f };
	registry.emplace<MoveComponent>(player, 6.0f);
	registry.emplace<NameComponent>(player, "Player");
	AnimatedMovementComponent& amc = registry.emplace<AnimatedMovementComponent>(player, Sprites::animPlayerUp, Sprites::animPlayerDown, Sprites::animPlayerLeft, Sprites::animPlayerRight, 0.1f);


	// Camera
	float aspectRatio = static_cast<float>(Game::Instance().GetWindow()->GetHeight()) / Game::Instance().GetWindow()->GetWidth();
	cameraController = std::make_shared<FreeRoamEntityCameraController>(aspectRatio, 1.0f);
	cameraController->SetZoomLevel(10);
	cameraController->SetPosition({ cameraController->GetZoomLevel() * 0.75, cameraController->GetZoomLevel() });

	// set free roam entity
	static_cast<FreeRoamEntityCameraController*>(cameraController.get())->SetEntity(&registry, player);
}


Level::~Level()
{
	delete world;
}

void shoot(entt::registry& registry, const glm::vec2& origin, const glm::vec2& direction)
{
	// Create entity
	entt::entity fireball = registry.create();
	registry.emplace<TransformComponent>(fireball, glm::vec3{ origin , 0.5f });
	registry.emplace<NameComponent>(fireball, "Fireball");
	MoveComponent& mc = registry.emplace<MoveComponent>(fireball, 10.0f);
	mc.moveVec = glm::normalize(direction - origin);
	registry.emplace<VisualComponent>(fireball, Sprites::fire);
	registry.emplace<TimeLifeComponent>(fireball, 1.0f);
}


glm::vec2 getMousePosInWorld(const std::shared_ptr<CameraController>& cameraController)
{
	glm::vec2 mousePos = Input::GetMousePosition();
	glm::vec2 cameraPos = cameraController->GetPosition();
	auto& bounds = cameraController->GetBounds();

	glm::vec2 mousePosPercent = { mousePos.x / Game::Instance().GetWindow()->GetWidth(),
						 mousePos.y / Game::Instance().GetWindow()->GetHeight() };


	glm::vec2 zoomedDimensions{ bounds.Right, -bounds.Top };

	return (glm::vec2{ mousePosPercent.x * zoomedDimensions.x - bounds.Right / 2.0f, mousePosPercent.y * zoomedDimensions.y + bounds.Top / 2.0f } *2.0f) + cameraController->GetPosition();
}

void Level::OnTick(Timestep delta)
{
	// Keyboard movement of player
	{
		auto [playerTransform, playerMove] = registry.get<TransformComponent, MoveComponent>(player);
		playerMove.zero();
		glm::vec2 move{ 0.0f };

		if (Input::IsKeyPressed(Input::KEY_W))
			move.y = 1.0f;
		if (Input::IsKeyPressed(Input::KEY_S))
			move.y = -1.0f;
		if (Input::IsKeyPressed(Input::KEY_A))
			move.x = -1.0f;
		if (Input::IsKeyPressed(Input::KEY_D))
			move.x = 1.0f;

		playerMove.updateMoveVec(move);
	}

	if (Input::IsKeyPressed(Input::KEY_SPACE))
	{
		if (shootTimer >= 1.0f || shootTimer == 0.0f)
		{
			shootTimer = std::max(shootTimer - 1.0f, 0.0f);
			LOG_TRACE("Shooting!");
			glm::vec3& playerPos = registry.get<TransformComponent>(player).position;

			shoot(registry, playerPos, getMousePosInWorld(cameraController));
		}
		shootTimer += delta;
	}
	else
		shootTimer = 0.0f;

	// Update positions based on move component
	{
		auto view = registry.view<TransformComponent, MoveComponent>();

		for (entt::entity e : view)
		{
			auto [transform, move] = view.get(e);
			transform.position += glm::vec3{ move.moveVec * move.speed * (float)delta, 0.0f };
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
				tlc.timeRemaining -= delta;
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
				amc.update(delta);
				vc.spriteId = amc.getCurrentSprite();

			}
		}
	}

	cameraController->OnUpdate(delta);

}


void Level::OnRender(Timestep delta)
{
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

		Renderer::DrawQuad({ 0, 0, 0.5f }, Sprites::get(Sprites::animPlayerUp[((int)Platform::GetElapsedTime()) % 4]));
		Renderer::DrawQuad({ 1, 0, 0.5f }, Sprites::get(Sprites::animPlayerDown[((int)Platform::GetElapsedTime()) % 4]));
		Renderer::DrawQuad({ 2, 0, 0.5f }, Sprites::get(Sprites::animPlayerLeft[((int)Platform::GetElapsedTime()) % 4]));
		Renderer::DrawQuad({ 3, 0, 0.5f }, Sprites::get(Sprites::animPlayerRight[((int)Platform::GetElapsedTime()) % 4]));
	}


	// entities
	{
		auto view = registry.view<TransformComponent, VisualComponent>();

		for (entt::entity e : view)
		{
			auto [transform, visual] = view.get(e);
			glm::vec3 drawTransform = { transform.position.x + visual.localTransform.x, transform.position.y + visual.localTransform.y, RenderDepth::ENTITY };
			Renderer::DrawQuad(drawTransform, Sprites::get(visual.spriteId), { 1, 1 });
		}
	}



	Renderer::EndScene();
}

void Level::OnImGuiRender(Timestep delta)
{
	ImGui::SetNextWindowBgAlpha(0.6f);
	ImGui::Begin("Level");

	ImGui::SeparatorText("Camera");
	if (ImGui::DragFloat2("pos##1", (float*)&cameraController->GetPosition()))
		cameraController->SetPosition(cameraController->GetPosition());
	if (ImGui::DragFloat("zoom", (float*)&cameraController->GetZoomLevel()))
		cameraController->SetZoomLevel(cameraController->GetZoomLevel());

	ImGui::Text("Bounds: (%.1f, %.1f)", cameraController->GetBounds().Right*2, cameraController->GetBounds().Top*2);

	glm::vec2 mousePos = getMousePosInWorld(cameraController);
	ImGui::Text("Mouse in world: (%.2f, %.2f)", mousePos.x, mousePos.y);

	ImGui::SeparatorText("Player");
	ImGui::DragFloat3("pos##2", (float*)&registry.get<TransformComponent>(player).position);
	ImGui::Text("moving: %s", registry.get<MoveComponent>(player).isMoving() ? "Yes" : "No");
	ImGui::Text("Shoot timer: %.2f", shootTimer);

	ImGui::SeparatorText("Entity");
	ImGui::Text("Entity count: %d", registry.view<TransformComponent>().size());

	ImGui::End();
	ImGui::SetNextWindowBgAlpha(1.0f);
}
