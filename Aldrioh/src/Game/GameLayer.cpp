#include "pch.h"
#include "GameLayer.h"

#include "Game.h"
#include "Core/Platform.h"

#include "Graphics/Renderer.h"
#include "Graphics/SubTexture.h"

#include "Input/Input.h"

#include "Game/Level/Level.h"
#include "Game/SpriteCollection.h"

#include "entt.hpp"

#include "Math/Math.h"

#include "Scene/Components.h"
#include "Scene/EntityCameraController.h"
#include "Scene/FreeRoamEntityCameraController.h"
#include "Components/Collision.h"

#include "imgui.h"

static entt::registry registry;
static entt::entity player;

static std::shared_ptr<Level> level;

static Ref<Shader> collisionShader;

static std::vector<entt::entity> entityRenderOrder;

GameLayer::GameLayer() {}

void GameLayer::OnBegin()
{
	// Add event handlers
	level = std::make_shared<Level>();
	SpriteCollection::init();

	collisionShader = CreateRef<Shader>("assets/shaders/TextureCoordReplaceColour.glsl");
	float aspectRatio = static_cast<float>(Game::Instance().GetWindow()->GetHeight()) / Game::Instance().GetWindow()->GetWidth();
	cameraController = std::make_shared<FreeRoamEntityCameraController>(aspectRatio, 1.0f);
	cameraController->SetZoomLevel(5);
	cameraController->SetPosition({ cameraController->GetZoomLevel() * 0.75, cameraController->GetZoomLevel() });

	player = registry.create();
	TransformComponent& tc = registry.emplace<TransformComponent>(player);
	tc.position.z = 0.4f;
	tc.position.x = 0.0f;
	tc.position.y = 0.0f;
	registry.emplace<VisualComponent>(player, SpriteCollection::player_head);
	registry.emplace<MoveComponent>(player, 6.0f);
	registry.emplace<NameComponent>(player, "Player");

	// set free roam entity
	static_cast<FreeRoamEntityCameraController*>(cameraController.get())->SetEntity(&registry, player);
}

static float shootTimer = 0.0f;

void shoot(const glm::vec2& origin, const glm::vec2& direction)
{
	// Create entity
	entt::entity fireball = registry.create();
	registry.emplace<TransformComponent>(fireball, glm::vec3{ origin , 0.5f });
	registry.emplace<NameComponent>(fireball, "Fireball");
	MoveComponent& mc = registry.emplace<MoveComponent>(fireball, 10.0f);
	mc.moveVec = glm::normalize(direction - origin);
	registry.emplace<VisualComponent>(fireball, SpriteCollection::fire);
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

	return (glm::vec2{ mousePosPercent.x * zoomedDimensions.x - bounds.Right/2.0f, mousePosPercent.y * zoomedDimensions.y + bounds.Top / 2.0f } * 2.0f) + cameraController->GetPosition();
}

void GameLayer::OnUpdate(Timestep delta)
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


	if (Input::IsKeyPressed(Input::KEY_SPACE))
	{
		if (shootTimer >= 1.0f || shootTimer == 0.0f)
		{
			shootTimer = std::max(shootTimer - 1.0f, 0.0f);
			LOG_TRACE("Shooting!");
			glm::vec3& playerPos = registry.get<TransformComponent>(player).position;

			shoot(playerPos, getMousePosInWorld(cameraController));
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
	Renderer::StartScene(cameraController->GetCamera().GetViewProjection());

	// Rendering visual components

	{
		auto view = registry.view<TransformComponent, VisualComponent>();

		for (entt::entity e : view)
		{
			auto [transform, visual] = view.get(e);
			Renderer::DrawQuad(transform.position + visual.localTransform, SpriteCollection::get(visual.spriteId), { 1, 1 });
			Renderer::DrawQuad(transform.position + visual.localTransform, SpriteCollection::get(visual.spriteId), { 0.5f, 0.5f });
		}

	}

	{
		glm::vec2 mousePos = Input::GetMousePosition();
		glm::vec2 cameraPos = cameraController->GetPosition();

		glm::vec2 worldPos = getMousePosInWorld(cameraController);

		Renderer::DrawQuad({ worldPos, 0.5f }, SpriteCollection::get(SpriteCollection::target), { 8.0f, 8.0f });
		Renderer::DrawQuad({ 0, 0, 0 }, SpriteCollection::get(SpriteCollection::null));

		for (int y = 0; y < 16; ++y)
		{
			for (int x = 0; x < 16; ++x)
			{
				Renderer::DrawQuad({ x, y, 0.5f }, SpriteCollection::get((x + y) % 7));
			}
		}
	}

	Renderer::EndScene();
}

void GameLayer::OnImGuiRender(Timestep delta)
{

	Game& game = Game::Instance();
	ImGuiIO& io = ImGui::GetIO();

	ImGui::SetNextWindowBgAlpha(0.4f);
	ImGui::Begin("Main Window");

	ImGui::SeparatorText("Game/Window");
	ImGui::Text("frame time: %.2f (%dfps)", delta.GetMilliSeconds(), game.gameStats.fps);
	ImGui::Text("Elapsed time: %.2f", Platform::GetElapsedTime());
	ImGui::Text("Blocking events: %s", ImGui::IsWindowFocused() ? "Yes" : "No");
	//game.BlockEvents(ImGui::IsWindowFocused());
	bool vsync = game.GetWindow()->GetVsync();
	if (ImGui::Checkbox("vsync", &vsync))
		game.GetWindow()->SetVsync(vsync);
	bool navActive = io.NavActive;
	ImGui::Checkbox("ImGui Nav Active", &navActive);
	bool wantCaptureKeyboard = io.WantCaptureKeyboard;
	ImGui::Checkbox("ImGui capture keyboard", &wantCaptureKeyboard);
	bool wantCaptureMouse = io.WantCaptureMouse;
	ImGui::Checkbox("ImGui capture mouse", &wantCaptureMouse);

	ImGui::SeparatorText("Camera");
	if (ImGui::DragFloat2("pos##1", (float*)&cameraController->GetPosition()))
		cameraController->SetPosition(cameraController->GetPosition());
	if (ImGui::DragFloat("zoom", (float*)&cameraController->GetZoomLevel()))
		cameraController->SetZoomLevel(cameraController->GetZoomLevel());

	ImGui::SeparatorText("Renderer");
	bool renderDepth = Renderer::IsRenderDepth();
	if (ImGui::Checkbox("Render depth", &renderDepth))
		Renderer::SetRenderDepthOnly(renderDepth);

	ImGui::SeparatorText("Entity");
	ImGui::Text("Entity count: %d", registry.view<TransformComponent>().size());

	ImGui::SeparatorText("Player");
	ImGui::DragFloat3("pos##2", (float*)&registry.get<TransformComponent>(player).position);
	ImGui::Text("moving: %s", registry.get<MoveComponent>(player).isMoving() ? "Yes" : "No");
	ImGui::Text("Shoot timer: %.2f", shootTimer);

	ImGui::SeparatorText("Input");
	ImGui::Text("Mouse Pos: (%.0f, %.0f)", Input::GetMouseX(), Input::GetMouseY());

	ImGui::End();
	ImGui::SetNextWindowBgAlpha(1.0f);

}

void GameLayer::OnRemove()
{
}