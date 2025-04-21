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

Level::Level()
{
	width = 25;
	height = 200;
	world = new int[width * height];

	for (int i = 0; i < width * height; i++)
	{
		world[i] = Sprites::sand_1;
	}

	player = registry.create();
	TransformComponent& tc = registry.emplace<TransformComponent>(player);
	tc.position.z = 0.4f;
	tc.position.x = 0.0f;
	tc.position.y = 0.0f;
	VisualComponent& pvc = registry.emplace<VisualComponent>(player, Sprites::player_head);
	pvc.localTransform = { -0.5f, -0.5f, 0.0f };
	registry.emplace<MoveComponent>(player, 6.0f);
	registry.emplace<NameComponent>(player, "Player");

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
	auto [playerTransform, playerMove] = registry.get<TransformComponent, MoveComponent>(player);
	playerMove.zero();

	if (Input::IsKeyPressed(Input::KEY_W))
		playerMove.moveVec.y = 1.0f;
	if (Input::IsKeyPressed(Input::KEY_S))
		playerMove.moveVec.y = -1.0f;
	if (Input::IsKeyPressed(Input::KEY_A))
		playerMove.moveVec.x = -1.0f;
	if (Input::IsKeyPressed(Input::KEY_D))
		playerMove.moveVec.x = 1.0f;

	glm::vec2 windowPos = Game::Instance().GetWindow()->GetPos();
	glm::vec2 windowMove{ 0.0f, 0.0f };

	if (Input::IsKeyPressed(Input::KEY_I))
		windowMove.y = -1.0f;
	if (Input::IsKeyPressed(Input::KEY_K))
		windowMove.y = 1.0f;
	if (Input::IsKeyPressed(Input::KEY_J))
		windowMove.x = -1.0f;
	if (Input::IsKeyPressed(Input::KEY_L))
		windowMove.x = 1.0f;

	windowMove = windowPos + windowMove * 10.0f;
	Game::Instance().GetWindow()->SetPos(windowMove.x, windowMove.y);


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

	cameraController->OnUpdate(delta);

	// Rendering


}


void Level::OnRender(Timestep delta)
{
	Renderer::StartScene(cameraController->GetCamera().GetViewProjection());

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int tile = world[y * width + x];
			glm::vec3 renderPos = { x * 1.0f, y * 1.0f, 0.0f };
			Renderer::DrawQuad(renderPos, Sprites::get(tile), { 1, 1 });
		}

	}

	// entities
	{
		auto view = registry.view<TransformComponent, VisualComponent>();

		for (entt::entity e : view)
		{
			auto [transform, visual] = view.get(e);
			glm::vec3 drawTransform = { transform.position.x + visual.localTransform.x, transform.position.y + visual.localTransform.y, 0.9f };
			Renderer::DrawQuad(drawTransform, Sprites::get(visual.spriteId), { 1, 1 });
		}
	}

	{
		glm::vec2 mousePos = Input::GetMousePosition();
		glm::vec2 cameraPos = cameraController->GetPosition();

		Renderer::DrawQuad({ 0, 0, 0 }, Sprites::get(Sprites::null));
	}

	Renderer::EndScene();
}

void Level::OnImGuiRender(Timestep delta)
{
	ImGui::SetNextWindowBgAlpha(0.4f);
	ImGui::Begin("Level");
	ImGui::SeparatorText("Camera");
	if (ImGui::DragFloat2("pos##1", (float*)&cameraController->GetPosition()))
		cameraController->SetPosition(cameraController->GetPosition());
	if (ImGui::DragFloat("zoom", (float*)&cameraController->GetZoomLevel()))
		cameraController->SetZoomLevel(cameraController->GetZoomLevel());
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
