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

GameLayer::GameLayer() {}

void GameLayer::OnBegin()
{
    SpriteCollection::init();

    // camera
    cameraController = std::make_shared<FreeRoamEntityCameraController>(1920.0f / 1080.0f, 70.0f);
    cameraController->SetPosition({0, 0});

    entt::entity player = registry.create();
    registry.emplace<TransformComponent>(player);
    registry.emplace<MoveComponent>(player);
    

    ((FreeRoamEntityCameraController*)cameraController.get())->SetEntity(&registry, player);
    cameraController->SetZoomLevel(75);
}

void GameLayer::OnUpdate(Timestep delta)
{
    // Update Entities
    cameraController->OnUpdate(delta);

    // Rendering
    Renderer::StartScene(cameraController->GetCamera().GetViewProjection());

    Renderer::DrawQuad({0.0f, 0.0f, 1.0f}, SpriteCollection::get(SpriteCollection::slime) , {100, 100});

    Renderer::DrawQuadFlatColour(glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}, glm::vec2{32.0f, 32.0f});

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
    ImGui::Text("Blocking events: %s",ImGui::IsWindowFocused() ? "Yes" : "No");
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
    ImGui::DragFloat2("pos##1", (float*)&cameraController->GetPosition());
    if (ImGui::DragFloat("zoom", (float*)&cameraController->GetZoomLevel()))
        cameraController->SetZoomLevel(cameraController->GetZoomLevel());

    ImGui::SeparatorText("Renderer");
    bool renderDepth = Renderer::IsRenderDepth();
    if (ImGui::Checkbox("Render depth", &renderDepth))
        Renderer::SetRenderDepthOnly(renderDepth);
    
    ImGui::End();
    ImGui::SetNextWindowBgAlpha(1.0f);

}

void GameLayer::OnRemove()
{
}