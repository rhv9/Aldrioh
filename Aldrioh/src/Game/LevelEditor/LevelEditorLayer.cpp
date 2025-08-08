#include <pch.h>
#include "LevelEditorLayer.h"


#include <Game.h>
#include <Core/Platform.h>
#include <Input/Input.h>
#include <Graphics/Renderer.h>

#include <Game/SpriteCollection.h>
#include <Game/RenderDepth.h>

#include <imgui.h>

#include "Math/Math.h"
#include <Scene/Components.h>
#include <Scene/FreeRoamEntityCameraController.h>
#include <FreeCameraController.h>
#include "Components/Collision.h"

#include <Scene/Entity.h>

#include <Game/Systems/CollisionSystems.h>
#include <Game/Systems/AnimatedSystems.h>
#include <Game/Systems/MovementSystems.h>
#include <Game/Systems/EnemyAISystems.h>
#include <Game/Systems/PlayerControllerSystems.h>
#include <Game/Systems/TestSystems.h>
#include <Game/Systems/CoreEntitySystems.h>

#include <Game/Systems/LevelSystems.h>
#include <Game/Systems/RenderSystems.h>
#include <Graphics/RenderQueue.h>

#include <Game/Level/TestLevel.h>
#include <Game/Components/LevelComponents.h>

#include <Debug/GameDebugState.h>
#include <Debug/Statistics.h>

#include <ImGui/ImGuiWindows.h>
#include <Game/Tiles/TexturedTiles.h>

#include <UI/Font.h>

LevelEditorLayer::LevelEditorLayer()
{
}

void LevelEditorLayer::OnBegin()
{
	scene = std::make_shared<Scene>();

	// Camera
	float aspectRatio = static_cast<float>(Game::Instance().GetWindow()->GetHeight()) / Game::Instance().GetWindow()->GetWidth();
	CameraController* cameraController = new FreeCameraController(aspectRatio, 1.0f);
	cameraController->SetZoomLevel(3);
	cameraController->SetPosition({ cameraController->GetZoomLevel() * 0.75, cameraController->GetZoomLevel() });

	// Add camera component
	Entity cameraEntity = scene->CreateEntity("FreeCameraController");
	cameraEntity.AddComponent<CameraComponent>(cameraController);
	cameraEntity.RemoveComponent<TransformComponent>(); // TODO: Need to consider this pls

	// Level system
	Entity levelEntity = scene->CreateEntity("Test Level");
	levelEntity.AddComponent<LevelComponent>(new TestLevel(*scene));
	levelEntity.RemoveComponent<TransformComponent>(); // TODO: Need to consider this pls

	// On Update Systems
	scene->AddUpdateSystem(&EntitySystem::LevelUpdateSystem);
	scene->AddUpdateSystem(&EntitySystem::CoreEntitySystems);


	// On Render Systems
	scene->AddRenderSystem(&EntitySystem::LevelRenderSystem);
	scene->AddRenderSystem(&EntitySystem::EntityRenderSystem);

	Game::Instance().GetWindow()->MouseButtonPressedEventHandler += [&](MouseButtonPressedEventArg& arg)
		{
			auto view = scene->getRegistry().view<LevelComponent>();

			for (auto e : view)
			{
				auto& lc = view.get<LevelComponent>(e);
				TestLevel* level = (TestLevel*)lc.level;
				LOG_CORE_INFO("Level width:{} height:{}", level->width, level->height);

				glm::vec2 pos = scene->GetMousePosInScene();
				
				level->SetTile((int)pos.x, (int)pos.y, new TexturedTiles(Sprites::sand_cactus));
			}
		};

}

static glm::vec2 pos{ 0 }, size{ 900, 100 };

void LevelEditorLayer::OnUpdate(Timestep delta)
{
	scene->OnUpdate(delta);
	scene->OnRender(delta);

	Renderer::StartUIScene();

	//Renderer::UIDrawRectangle({ -1.0f, 0 }, { 1, 1 }, glm::vec4(1, 1, 0, 1));
	Renderer::UIDrawRectangle(pos, size, glm::vec4(1));
	Renderer::UIDrawText(Font::DEFAULT, "Hello There!!", { 0.0f, 0.0f }, { 50, 50 }, glm::vec4{ 0.7f, 0.0f, 0.7f, 1.0f });
	Renderer::EndUIScene();
}

void LevelEditorLayer::OnImGuiRender(Timestep delta)
{
	Game& game = Game::Instance();
	ImGuiIO& io = ImGui::GetIO();

	ImGui::SetNextWindowBgAlpha(0.6f);
	ImGui::Begin("Main Window");

	ImGui::DragFloat2("Rectangle Pos: ", (float*)&pos);
	ImGui::DragFloat2("Rectangle Size: ", (float*)&size);

	Renderer::ImGuiDebug();

	if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("Game"))
		{
			GameImGuiWindows::ShowWindowInfo();
			GameImGuiWindows::ShowGameInfo();
			GameImGuiWindows::ShowImGuiInfo();
			GameImGuiWindows::ShowRendererInfo();

			if (ImGui::CollapsingHeader("Level"))
			{
				// From level
				auto& cameraController = scene->GetPrimaryCameraEntity().GetComponent<CameraComponent>().cameraController;

				ImGui::SeparatorText("Camera");
				if (ImGui::DragFloat2("pos##1", (float*)&cameraController->GetPosition()))
					cameraController->SetPosition(cameraController->GetPosition());
				if (ImGui::DragFloat("zoom", (float*)&cameraController->GetZoomLevel()))
					cameraController->SetZoomLevel(cameraController->GetZoomLevel());

				ImGui::Text("Bounds: (%.1f, %.1f)", cameraController->GetBounds().Right * 2, cameraController->GetBounds().Top * 2);

				glm::vec2 mousePos = scene->GetMousePosInScene();
				ImGui::Text("Mouse in world: (%.2f, %.2f)", mousePos.x, mousePos.y);

				ImGui::SeparatorText("Entity");
				ImGui::Text("Entity count: %d", scene->getRegistry().view<TransformComponent>().size());
			}

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Level Editor"))
		{

			SubTexture* texture = Sprites::get(Sprites::animBossRight[1]);
			glm::vec2 uv1 = texture->GetTexCoords().bottomLeft;
			glm::vec2 uv0 = texture->GetTexCoords().topRight;

			ImGui::Image(texture->GetTextureId(), { (float)256, (float)256 }, {uv0.x, uv0.y}, {uv1.x, uv1.y});

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Other"))
		{
			if (ImGui::Button("Switch to Game Layer"))
			{
			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	ImGui::End();
	ImGui::SetNextWindowBgAlpha(1.0f);

	//ImGui::ShowDemoWindow();

	//ImGui::ShowMetricsWindow();

}

void LevelEditorLayer::OnRemove()
{

}
