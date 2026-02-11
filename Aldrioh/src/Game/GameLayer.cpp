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

#include <Game/Debug/GameDebugState.h>
#include <Debug/Statistics.h>

#include <ImGui/ImGuiWindows.h>

#include <Game/GlobalLayers.h>
#include <Audio/SoundManager.h>

#include <Game/Entity/GameEntityPrefab.h>



GameLayer::~GameLayer()
{
	Game::Instance().GetLayerStack().QueuePopLayer(GlobalLayers::gameUILayer);
}

void GameLayer::OnBegin()
{
	Renderer::SetClearColour({ 0.0f, 0.0f, 0.0f, 1.0f });

	scene = std::make_unique<Scene>();
	Game::Instance().GetLayerStack().QueuePushLayer(GlobalLayers::gameUILayer);
	
	// Level system
	currentLevel = std::make_unique<Level>(*scene);
	Entity levelEntity = scene->CreateEntityNoTransform("Level 1");
	levelEntity.AddComponent<LevelComponent>(currentLevel.get());
	currentLevel->UpdateLevelArea();

	// UI Component has to be before level
	Entity uiEntity = scene->CreateEntityNoTransform("UIManager");

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
	//scene->AddUpdateSystem(&EntitySystem::ResetAndAddCollisionWorld);
	scene->AddUpdateSystem(&EntitySystem::ResetAndAddCollisionZone);

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
	scene->AddRenderSystem(&EntitySystem::DebugRenderCollisionZoneVisualisation);

	// On UI Render Systems
	scene->AddUIRenderSystem(&EntitySystem::UIManagerRenderSystem);
}

void GameLayer::OnUpdate(Timestep delta)
{
	if (GameDebugState::shouldUpdateScene)
		scene->OnUpdate(delta);
}

void GameLayer::OnRender(Timestep delta)
{
	//Renderer::DrawBackgroundPass();
	scene->OnRender(delta);
	scene->OnUIRender(delta);
}

void GameLayer::OnImGuiRender(Timestep delta)
{
	static bool open = false;

	if (!GameDebugState::enabledImGui)
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
			currentLevel->ImGuiLevelBar(delta);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Waves"))
		{
			currentLevel->GetWaveManager().OnImGuiDebugging();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	ImGui::End();
	//ImGui::ShowDemoWindow();
}

void GameLayer::OnKeyEvent(KeyEventArg& e)
{
	scene->OnKeyEvent(e);
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
		GameDebugState::enabledImGui = !GameDebugState::enabledImGui;
}


void GameLayer::OnMouseButtonEvent(MouseButtonEventArg& e)
{
	scene->OnMouseButtonEvent(e);
}

void GameLayer::OnMouseMoveEvent(MouseMoveEventArg& e)
{
	scene->OnMouseMoveEvent(e);
}

void GameLayer::OnMouseScrolledEvent(MouseScrolledEventArg& e)
{
	scene->OnMouseScrollEvent(e);
}

void GameLayer::OnWindowResizeEvent(WindowResizeEventArg& e)
{
	scene->OnWindowResizeEvent(e);
	currentLevel->UpdateLevelArea();
}

void GameLayer::OnPlayerDeath()
{
	GlobalLayers::game->QueueTransitionTo(GlobalLayers::gameOver);
}

GameUILayer* GameLayer::GetUILayer()
{
	return GlobalLayers::gameUILayer;
}

void GameLayer::OnTransitionIn()
{
	scene->OnTransitionIn();
}

void GameLayer::OnTransitionOut()
{
	scene->OnTransitionOut();
}

void GameLayer::OnPushedLayerAboveEvent()
{
	OnTransitionOut();
}

void GameLayer::OnPoppedLayerIntoEvent()
{
	scene->OnTransitionIn();
}
