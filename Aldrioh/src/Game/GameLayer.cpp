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
    cameraController = std::make_shared<CameraController>(aspectRatio, 1.0f);

    cameraController->SetPosition({ 0, 0 });

    player = registry.create();
    TransformComponent& tc = registry.emplace<TransformComponent>(player);
    tc.position.z = 0.4f;
    tc.position.x = 0.0f;
    tc.position.y = 0.0f;
    registry.emplace<VisualComponent>(player, SpriteCollection::player_head);
    registry.emplace<MoveComponent>(player, 32.0f);
    registry.emplace<NameComponent>(player, "Player");
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

    playerTransform.position += glm::vec3{ playerMove.moveVec * playerMove.speed * (float)delta, 0.0f };


    // Rendering
    Renderer::StartScene(cameraController->GetCamera().GetViewProjection());

    // Rendering visual components

    {
        auto view = registry.view<TransformComponent, VisualComponent>();

        for (entt::entity e : view)
        {
            auto [transform, visual] = view.get(e);
            Renderer::DrawQuad(transform.position + visual.localTransform, SpriteCollection::get(SpriteCollection::target), SpriteCollection::Tile_size);
            Renderer::DrawQuad(transform.position + visual.localTransform, SpriteCollection::get(visual.spriteId), {1, 1});
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

    ImGui::SeparatorText("Player");
    ImGui::DragFloat3("pos##2", (float*)&registry.get<TransformComponent>(player).position);
    ImGui::Text("moving: %s", registry.get<MoveComponent>(player).isMoving() ? "Yes" : "No");

    ImGui::End();
    ImGui::SetNextWindowBgAlpha(1.0f);

}

void GameLayer::OnRemove()
{
}