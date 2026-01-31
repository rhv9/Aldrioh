#include "pch.h"
#include "GameLayer.h"

#include "Game.h"
#include "Core/Platform.h"
#include "Input/Input.h"
#include "Graphics/Renderer.h"

#include "Game/SpriteCollection.h"
#include "RenderDepth.h"

#include <imgui.h>

#include "Math/Math.h"
#include <Scene/Components.h>
#include <Scene/FreeRoamEntityCameraController.h>
#include "Collision/Collision.h"

#include <Scene/Entity.h>

#include <Game/Systems/CollisionSystems.h>
#include <Game/Systems/AnimatedSystems.h>
#include <Game/Systems/MovementSystems.h>
#include <Game/Systems/EnemyAISystems.h>
#include <Game/Systems/PlayerControllerSystems.h>
#include <Game/Systems/TestSystems.h>
#include <Game/Systems/CoreEntitySystems.h>
#include <Game/Systems/ControllerSystems.h>
#include <Systems/UISystems.h>
#include <Systems/SoundSystems.h>
#include <Systems/PathSystems.h>

#include <Game/Systems/LevelSystems.h>
#include <Game/Systems/RenderSystems.h>
#include <Graphics/RenderQueue.h>

#include <Game/Components/LevelComponents.h>

#include <Debug/GameDebugState.h>
#include <Debug/Statistics.h>

#include <ImGui/ImGuiWindows.h>

#include <Game/GlobalLayers.h>
#include <Audio/SoundManager.h>

#include <Game/Entity/GameEntityPrefab.h>

struct ImGuiSettings
{
	bool enabledImGui = false;

	bool shouldUpdateScene = true;
	bool shouldPathRecord = false;
	bool pathStartStopHovered = false;

	bool EnabledDebugCamera = false;
};

struct LevelEditorData
{
	std::vector<glm::vec2> points;
};

static ImGuiSettings imGuiSettings;
static LevelEditorData levelEditorData;


void GameLayer::OnBegin()
{
	Renderer::SetClearColour({ 0.0f, 0.0f, 0.0f, 1.0f });

	scene = std::make_unique<Scene>();

	// Level system
	currentLevel = std::make_unique<Level>(*scene);
	Entity levelEntity = scene->CreateEntityNoTransform("Level 1");
	levelEntity.AddComponent<LevelComponent>(currentLevel.get());
	currentLevel->UpdateLevelArea();

	// UI Component has to be before level
	Entity uiEntity = scene->CreateEntityNoTransform("UIManager");
	UIManagerComponent& uimc = uiEntity.AddComponent<UIManagerComponent>();
	uimc.uiManager = std::make_unique<UIManager>();

	// UI
	uiScoreText = new UIText("Score", { 2, 2 }, glm::vec2{ 0 });
	uiScoreText->SetText("Score: 0");
	uiScoreText->SetAnchorPoint(AnchorPoint::LEFT_TOP);
	uiScoreText->GetFontStyle().colour = Colour::WHITE;
	uiScoreText->SetFontSize(4);
	uimc.uiManager->AddUIObject(uiScoreText);

	uiPlayerHealthBar = new UIProgressBar("Player Health", { 2, 2 }, { 7, 0.4f });
	uiPlayerHealthBar->SetAnchorPoint(AnchorPoint::CENTER);
	uiPlayerHealthBar->SetBackgroundColour(Colour::GREY);
	uiPlayerHealthBar->SetProgress(0.5f);
	uimc.uiManager->AddUIObject(uiPlayerHealthBar);

	// UI
	uiTimerText = new UIText("Timer", { 0, 4 }, glm::vec2{ 4, 4 });
	uiTimerText->SetText("");
	uiTimerText->SetAnchorPoint(AnchorPoint::CENTER_TOP);
	uiTimerText->GetFontStyle().colour = Colour::WHITE;
	uiTimerText->GetFontStyle().charSpacingPercent = 0.9f;
	uiTimerText->SetFontSize(4);
	uimc.uiManager->AddUIObject(uiTimerText);

	// On Update Systems
	scene->AddUpdateSystem(&EntitySystem::ResetMovementSystem);
	scene->AddUpdateSystem(&EntitySystem::PlayerControllerSystem);
	scene->AddUpdateSystem(&EntitySystem::DumbAISystem);
	scene->AddUpdateSystem(&EntitySystem::FollowPlayerAISystem);
	scene->AddUpdateSystem(&EntitySystem::ControllerSystems);
	scene->AddUpdateSystem(&EntitySystem::PathsSystem);

	scene->AddUpdateSystem(&EntitySystem::JumpSystem);
	scene->AddUpdateSystem(&EntitySystem::LifeSystem);
	scene->AddUpdateSystem(&EntitySystem::HealthSystem);
	scene->AddUpdateSystem(&EntitySystem::AnimatedMovementSystem);
	scene->AddUpdateSystem(&EntitySystem::LevelUpdateSystem);
	scene->AddUpdateSystem(&EntitySystem::TestUpdateSystem);

	// After rest of updates are done
	scene->AddUpdateSystem(&EntitySystem::ResetAndAddCollisionWorld);
	scene->AddUpdateSystem(&EntitySystem::MovementSystem);
	//scene->AddUpdateSystem(&EntitySystem::CollisionSystem);
	scene->AddUpdateSystem(&EntitySystem::CoreEntitySystems);
	scene->AddUpdateSystem(&EntitySystem::DeleteEnemyOutsideScreenSystem);

	// Very last
	scene->AddUpdateSystem(&EntitySystem::RotationSystem);

	// Not game system 
	scene->AddUpdateSystem(&EntitySystem::UIManagerUpdateSystem);

	// Does not matter order
	scene->AddUpdateSystem(&EntitySystem::ScoreSystems);
	scene->AddUpdateSystem(&EntitySystem::SoundSystem);


	// On Render Systems
	scene->AddRenderSystem(&EntitySystem::LevelRenderSystem);
	scene->AddRenderSystem(&EntitySystem::TestRenderSystem);
	scene->AddRenderSystem(&EntitySystem::EntityRenderSystem);
	scene->AddRenderSystem(&EntitySystem::CollisionRenderSystem);


	// On UI Render Systems
	scene->AddUIRenderSystem(&EntitySystem::UIManagerRenderSystem);
}

void GameLayer::OnUpdate(Timestep delta)
{
	if (imGuiSettings.shouldUpdateScene)
		scene->OnUpdate(delta);
}

void GameLayer::OnRender(Timestep delta)
{
	//Renderer::DrawBackgroundPass();
	scene->OnRender(delta);
	scene->OnUIRender(delta);

	// Render Pass for drawing LevelEditing pointers
	auto& cameraController = scene->GetPrimaryCameraEntity().GetComponent<CameraComponent>().cameraController;
	Renderer::StartScene({ cameraController->GetCamera().GetViewProjection() });

	int i = 0;
	for (const glm::vec2& point : levelEditorData.points)
	{
		constexpr glm::vec2 size = { 0.45f, 0.45f };
		Renderer::DrawQuad(glm::vec3{ point - size/2.0f, 1.0f }, Font::DEFAULT->GetCharSubTexture(i++ + '0'), size, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0, 1);
	}

	auto offset = currentLevel->GetScreenBorderOffsetByCamera(currentLevel->GetPlayerCamera().GetComponent<CameraComponent>().cameraController->GetPosition());

	constexpr glm::vec2 size = { 0.45f, 0.45f };
	Renderer::DrawQuad(glm::vec3{ offset.bottomLeft - size / 2.0f, 1.0f }, Font::DEFAULT->GetBlockSubTexture(), size, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0, 1);
	Renderer::DrawQuad(glm::vec3{ offset.topRight - size / 2.0f, 1.0f }, Font::DEFAULT->GetBlockSubTexture(), size, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0, 1);
	
	Renderer::EndScene();
}

void GameLayer::OnImGuiRender(Timestep delta)
{
	static bool open = false;

	if (!imGuiSettings.enabledImGui)
		return;

	ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

	ImGui::Begin("Main Window", &open, ImGuiWindowFlags_NoFocusOnAppearing);

	if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("Game"))
		{
			GameImGuiWindows::ShowGameInfo();
			GameImGuiWindows::ShowImGuiInfo();
			GameImGuiWindows::ShowRendererInfo();
			scene->OnImGuiDebugRender(delta);

			if (ImGui::CollapsingHeader("Level", ImGuiTreeNodeFlags_DefaultOpen))
			{
				// From level
				auto& cameraController = scene->GetPrimaryCameraEntity().GetComponent<CameraComponent>().cameraController;

				ImGui::SeparatorText("Camera");
				ImGui::PushItemWidth(100);
				if (ImGui::DragFloat2(" RelativePos##1", (float*)&cameraController->GetPosition()))
					cameraController->SetPosition(cameraController->GetPosition());
				if (ImGui::DragFloat(" Zoom", (float*)&cameraController->GetZoomLevel()))
					cameraController->SetZoomLevel(cameraController->GetZoomLevel());
				ImGui::PopItemWidth();
				ImGui::Text("Bounds: (%.1f, %.1f)", cameraController->GetBounds().Right * 2, cameraController->GetBounds().Top * 2);

				glm::vec2 mousePos = scene->GetMousePosInScene();
				ImGui::Text("Mouse in world: (%.2f, %.2f)", mousePos.x, mousePos.y);

				ImGui::SeparatorText("Entity");
				ImGui::Text("Entity count: %d", scene->getRegistry().view<TransformComponent>().size());
			}

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Level"))
		{
			currentLevel->ImGuiLevelBar();

			if (ImGui::Checkbox("Debugging Camera", &imGuiSettings.EnabledDebugCamera))
				currentLevel->SetEnableDebugCamera(imGuiSettings.EnabledDebugCamera);

			if (ImGui::Button(imGuiSettings.shouldUpdateScene ? "Pause" : "Play"))
				imGuiSettings.shouldUpdateScene = !imGuiSettings.shouldUpdateScene;

			ImGui::SeparatorText("Entity Pathing");

			if (ImGui::Button(imGuiSettings.shouldPathRecord ? "Stop recording path" : "Start recording path"))
				imGuiSettings.shouldPathRecord = !imGuiSettings.shouldPathRecord;

			if (ImGui::IsItemHovered())
				imGuiSettings.pathStartStopHovered = true;
			else
				imGuiSettings.pathStartStopHovered = false;

			if (ImGui::Button("Reset Path"))
				levelEditorData.points.clear();

			static float pathEnemySpeed = 1.0f;
			ImGui::DragFloat("Initial Speed", &pathEnemySpeed);


			if (ImGui::Button("Create enemy"))
			{
				LOG_INFO("Creating a path enemy!");
				EnemyPathPrefab epp;
				epp.points = levelEditorData.points;
				epp.speed = pathEnemySpeed;
				epp.create(currentLevel->scene);
			}

			static bool keepCreating = false;
			static float interval = 1.0f;
			static float elapsedTime = 0.0f;
			elapsedTime += delta;

			ImGui::DragFloat("Interval", &interval);

			if (ImGui::Button(keepCreating ? "Stop  da centipede" : "Begin epicness"))
				keepCreating = !keepCreating;

			if (elapsedTime >= interval)
			{
				elapsedTime -= interval;
				if (keepCreating)
				{
					EnemyPathPrefab epp;
					epp.points = levelEditorData.points;
					epp.speed = pathEnemySpeed;
					epp.create(currentLevel->scene);
				}
			}

			if (levelEditorData.points.size() != 0 || imGuiSettings.shouldPathRecord)
			{
				std::string text;
				text.reserve(512);

				for (const glm::vec2& point : levelEditorData.points)
				{
					text.append(std::format(",({:.1f},{:.1f})", point.x, point.y));
				}

				ImGui::Text(std::format("Path: {}", text).c_str());
			}

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	ImGui::End();
	//ImGui::ShowDemoWindow();
}

void GameLayer::OnRemove()
{
}

void GameLayer::OnKey(KeyEventArg& e)
{
	if (e.IsPressed(Input::KEY_ESCAPE))
	{
		LOG_INFO("Pushing pause menu layer");
		Game::Instance().GetLayerStack().QueuePushLayer(GlobalLayers::pauseMenu);
	}

	if (e.IsPressed(Input::KEY_P))
	{
		SoundManager::Play("sfx");
	}
	if (e.IsPressed(Input::KEY_GRAVE_ACCENT))
		imGuiSettings.enabledImGui = !imGuiSettings.enabledImGui;
}

void GameLayer::OnWindowResize(WindowResizeEventArg& e)
{
	if (currentLevel != nullptr)
		currentLevel->UpdateLevelArea();
}

void GameLayer::OnMouseButton(MouseButtonEventArg& e)
{
	if (e.IsReleased(Input::MOUSE_BUTTON_LEFT))
	{
		if (imGuiSettings.shouldPathRecord && !imGuiSettings.pathStartStopHovered)
		{
			levelEditorData.points.push_back(scene->GetMousePosInScene());
		}
	}
}


void GameLayer::OnTransitionIn()
{
	LOG_CORE_INFO("Game Layer - Transition In");
	SetShouldUpdate(true);
	SetShouldRender(true);
	callbackKeyID = Game::Instance().GetWindow()->KeyEventHandler += EVENT_BIND_MEMBER_FUNCTION(GameLayer::OnKey);
	windowResizeID = Game::Instance().GetWindow()->WindowResizeEventHandler += EVENT_BIND_MEMBER_FUNCTION(GameLayer::OnWindowResize);
	callbackMouseButtonID = Game::Instance().GetWindow()->MouseButtonEventHandler += EVENT_BIND_MEMBER_FUNCTION(GameLayer::OnMouseButton);

	scene->OnTransitionIn();
}

void GameLayer::OnTransitionOut()
{
	LOG_CORE_INFO("Game Layer - Transition Out");
	callbackKeyID.~EventCallbackID();
	windowResizeID.~EventCallbackID();
	callbackMouseButtonID.~EventCallbackID();
	scene->OnTransitionOut();
}

void GameLayer::OnPushedLayerAboveEvent()
{
	LOG_CORE_INFO("Game Layer - PushedLayerAboveEvent");
	SetShouldUpdate(false);
	OnTransitionOut();
}

void GameLayer::OnPoppedLayerIntoEvent()
{
	LOG_CORE_INFO("Game Layer - PoppedLayerAboveEvent");
	SetShouldUpdate(true);
	SetShouldRender(true);
	callbackKeyID = Game::Instance().GetWindow()->KeyEventHandler += EVENT_BIND_MEMBER_FUNCTION(GameLayer::OnKey);
	windowResizeID = Game::Instance().GetWindow()->WindowResizeEventHandler += EVENT_BIND_MEMBER_FUNCTION(GameLayer::OnWindowResize);
	callbackMouseButtonID = Game::Instance().GetWindow()->MouseButtonEventHandler += EVENT_BIND_MEMBER_FUNCTION(GameLayer::OnMouseButton);

	scene->OnTransitionIn();
}
