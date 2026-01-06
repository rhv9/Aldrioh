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
#include "Collision/Collision.h"

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

#include <UI/UIManager.h>
#include <UI/UIObject.h>
#include <UI/UIText.h>
#include <UI/UIImage.h>
#include <UI/UIButton.h>

LevelEditorLayer::LevelEditorLayer()
{
}

static UIManager* uiManager = nullptr;

EventCallbackID<MouseButtonPressedEventArg> callbackMousePressedID;

void LevelEditorLayer::OnBegin()
{
	//scene = std::make_shared<Scene>();

}


void LevelEditorLayer::OnUpdate(Timestep delta)
{
	//scene->OnUpdate(delta);
	//uiManager->OnUpdate(delta);
}

void LevelEditorLayer::OnRender(Timestep delta)
{
	//scene->OnRender(delta);
}


void LevelEditorLayer::OnImGuiRender(Timestep delta)
{
	Game& game = Game::Instance();
	ImGuiIO& io = ImGui::GetIO();
}

void LevelEditorLayer::OnRemove()
{

}
