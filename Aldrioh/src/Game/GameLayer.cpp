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
#include "Components/Collision.h"

#include <Scene/Entity.h>

#include <Game/Systems/CollisionSystems.h>
#include <Game/Systems/AnimatedSystems.h>
#include <Game/Systems/MovementSystems.h>
#include <Game/Systems/EnemyAISystems.h>
#include <Game/Systems/PlayerControllerSystems.h>
#include <Game/Systems/TestSystems.h>
#include <Game/Systems/CoreEntitySystems.h>
#include <Systems/UISystems.h>
#include <Systems/SoundSystems.h>

#include <Game/Systems/LevelSystems.h>
#include <Game/Systems/RenderSystems.h>
#include <Graphics/RenderQueue.h>

#include <Game/Level/Level1.h>
#include <Game/Components/LevelComponents.h>

#include <Debug/GameDebugState.h>
#include <Debug/Statistics.h>

#include <ImGui/ImGuiWindows.h>

#include <Game/GlobalLayers.h>
#include <Audio/SoundManager.h>

GameLayer::GameLayer() {}

void GameLayer::OnBegin()
{
	Renderer::SetClearColour({ 0.0f, 0.0f, 0.0f, 1.0f });

	scene = std::make_shared<Scene>();

	// UI Component has to be before level
	Entity uiEntity = scene->CreateEntityNoTransform("UIManager");
	UIManagerComponent& uimc = uiEntity.AddComponent<UIManagerComponent>();
	uimc.uiManager = std::make_unique<UIManager>();

	// Level system
	Entity levelEntity = scene->CreateEntityNoTransform("Level 1");
	levelEntity.AddComponent<LevelComponent>(new Level1(*scene));

	// UI
	uiScoreText = new UIText("Score", { 2, 2 }, glm::vec2{ 0 });
	uiScoreText->SetText("Score: 0");
	uiScoreText->SetAnchorPoint(AnchorPoint::LEFT_TOP);
	uiScoreText->GetFontStyle().colour = Colour::WHITE;
	uiScoreText->SetFontSize(4);
	uimc.uiManager->AddUIObject(uiScoreText);


	// On Update Systems
	//scene->AddUpdateSystem(&EntitySystem::ResetMovementSystem);
	scene->AddUpdateSystem(&EntitySystem::PlayerControllerSystem);
	scene->AddUpdateSystem(&EntitySystem::JumpSystem);
	scene->AddUpdateSystem(&EntitySystem::LifeSystem);
	scene->AddUpdateSystem(&EntitySystem::HealthSystem);
	scene->AddUpdateSystem(&EntitySystem::DumbAISystem);
	scene->AddUpdateSystem(&EntitySystem::AnimatedMovementSystem);
	scene->AddUpdateSystem(&EntitySystem::ScoreSystems);
	scene->AddUpdateSystem(&EntitySystem::LevelUpdateSystem);
	scene->AddUpdateSystem(&EntitySystem::TestUpdateSystem);
	scene->AddUpdateSystem(&EntitySystem::MovementSystem);
	scene->AddUpdateSystem(&EntitySystem::CollisionSystem);
	scene->AddUpdateSystem(&EntitySystem::CoreEntitySystems);
	scene->AddUpdateSystem(&EntitySystem::UIManagerUpdateSystem);
	scene->AddUpdateSystem(&EntitySystem::SoundSystem);


	// On Render Systems
	scene->AddRenderSystem(&EntitySystem::LevelRenderSystem);
	scene->AddRenderSystem(&EntitySystem::TestRenderSystem);
	scene->AddRenderSystem(&EntitySystem::EntityRenderSystem);
	scene->AddRenderSystem(&EntitySystem::CollisionRenderSystem);


	// On UI Render Systems
	scene->AddUIRenderSystem(&EntitySystem::UIManagerRenderSystem);

	SoundManager::LoadSound(SoundCategory::SFX, "sfx", "assets/audio/sfx_exp_long4.wav");
	SoundManager::LoadSound(SoundCategory::SFX, "player_shoot", "assets/audio/General\ Sounds/High\ Pitched\ Sounds/sfx_sounds_high3.wav", 0.05f);
	SoundManager::LoadSound(SoundCategory::SFX, "bullet_impact", "assets/audio/General\ Sounds/Impacts/sfx_sounds_impact1.wav", 0.05f);

}

void GameLayer::OnUpdate(Timestep delta)
{
	scene->OnUpdate(delta);

}

void GameLayer::OnRender(Timestep delta)
{
	scene->OnRender(delta);
	scene->OnUIRender(delta);
}

void GameLayer::OnImGuiRender(Timestep delta)
{
	ImGui::SetNextWindowBgAlpha(0.6f);
	ImGui::Begin("Main Window");

	if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("Game"))
		{
			GameImGuiWindows::ShowGameInfo();
			GameImGuiWindows::ShowImGuiInfo();
			GameImGuiWindows::ShowRendererInfo();

			if (ImGui::CollapsingHeader("Level"))
			{
				// From level
				auto& cameraController = scene->GetPrimaryCameraEntity().GetComponent<CameraComponent>().cameraController;

				ImGui::SeparatorText("Camera");
				if (ImGui::DragFloat2("relativePos##1", (float*)&cameraController->GetPosition()))
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
		ImGui::EndTabBar();
	}

	ImGui::End();
	ImGui::SetNextWindowBgAlpha(1.0f);

	//ImGui::ShowDemoWindow();
}	

void GameLayer::OnRemove()
{
}

void GameLayer::OnKeyPressed(KeyPressedEventArg& e)
{
	if (e.Key == Input::KEY_ESCAPE)
	{
		LOG_INFO("Pushing pause menu layer");
		Game::Instance().GetLayerStack().PushLayer(GlobalLayers::pauseMenu);
		SetShouldUpdate(false);
		OnTransitionOut();
	}

	if (e.Key == Input::KEY_P)
	{
		SoundManager::Play("sfx");

	}
}

void GameLayer::OnTransitionIn()
{
	SetShouldUpdate(true);
	SetShouldRender(true);
	callbackKeyPressedID = Game::Instance().GetWindow()->KeyPressedEventHandler += EVENT_BIND_MEMBER_FUNCTION(GameLayer::OnKeyPressed);
	scene->OnTransitionIn();
}

void GameLayer::OnTransitionOut()
{
	callbackKeyPressedID.~EventCallbackID();
	scene->OnTransitionOut();
}
