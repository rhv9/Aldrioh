#include "pch.h"
#include "TestLayer.h"

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

TestLayer::TestLayer() {}

void TestLayer::OnBegin()
{
    // Add event handlers


    level = std::make_shared<Level>();
    Sprites::init();

    collisionShader = CreateRef<Shader>("assets/shaders/TextureCoordReplaceColour.glsl");

    // camera
    cameraController = std::make_shared<FreeRoamEntityCameraController>(1920.0f / 1080.0f, 70.0f);
    ((FreeRoamEntityCameraController*)cameraController.get())->SetEntity(&registry, player);
    cameraController->SetZoomLevel(75);

    // Do entities
    player = registry.create();
    TransformComponent& tc = registry.emplace<TransformComponent>(player);
    tc.position.z = 0.4f;
    registry.emplace<VisualComponent>(player, Sprites::player_head);
    registry.emplace<CollisionBox>(player, glm::vec3{ 0 }, glm::vec2{ 16.0f, 16.0f });
    registry.emplace<MoveComponent>(player, 32.0f * 3);
    registry.emplace<NameComponent>(player, "player");

    int entityCount = 0;

    for (int i = 0; i < 10; i++)
    {
        entt::entity entity = registry.create();

        registry.emplace<TransformComponent>(entity, glm::vec3{ Math::Random::linearInt(0, 16 * 10), Math::Random::linearInt(0, 16 * 10), 0.0f });
        registry.emplace<VisualComponent>(entity, Sprites::fire);
        registry.emplace<NameComponent>(entity, "fire");

    }

    // Add collision shape

    {
        entt::entity e1 = registry.create();
        registry.emplace<TransformComponent>(e1, glm::vec3{ 32.0f, 48.1f, 0.4f });
        registry.emplace<CollisionBox>(e1, glm::vec3{ 0 }, glm::vec2{ 16.0f, 16.0f });

        entt::entity e2 = registry.create();
        registry.emplace<TransformComponent>(e2, glm::vec3{ 32.0f, 32.0f, 0.4f });
        registry.emplace<CollisionBox>(e2, glm::vec3{ 0 }, glm::vec2{ 16.0f, 16.0f });
    }

    {
        // Add Random movement entities
        for (int i = 0; i < 12; i++)
        {
            entt::entity e = registry.create();
            registry.emplace<TransformComponent>(e, glm::vec3{ Math::Random::linearInt(0, 16 * 2), Math::Random::linearInt(0, 16 * 16) , 0.2f });
            registry.emplace<VisualComponent>(e, Sprites::slime);
            registry.emplace<RandomMovementComponent>(e, Math::Random::linearInt(6, 32));
            registry.emplace<NameComponent>(e, "slime");
            registry.emplace<CollisionBox>(e, glm::vec3{ 0 }, glm::vec2{ 16.0f, 16.0f });
        }
    }
}

void TestLayer::OnUpdate(Timestep delta)
{
    // Update Entities

    // Update player
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

    }


    // Update ai entities
    {
        auto view = registry.view<TransformComponent, RandomMovementComponent>();

        for (entt::entity e : view)
        {
            auto [transform, randomMove] = view.get<TransformComponent, RandomMovementComponent>(e);

            transform.position.x += randomMove.speed * delta;
        }
    }

    cameraController->OnUpdate(delta);

    // Rendering
    Renderer::StartScene(cameraController->GetCamera().GetViewProjection());

    level->OnTick(delta);
    level->OnRender(delta);

    // Rendering visual components
    {
        auto view = registry.view<TransformComponent, VisualComponent>();
        entityRenderOrder.clear();
        for (entt::entity entity : view)
        {
            auto [transform, visual] = view.get(entity);
            Renderer::DrawQuad(transform.position + visual.localTransform, Sprites::get(visual.spriteId), Sprites::TileSize);
            entityRenderOrder.push_back(entity);
        }
    }

    auto collisionView = registry.view<TransformComponent, CollisionBox>();
    for (entt::entity entity : collisionView)
    {
        auto [transform, collisionBox] = collisionView.get(entity);
        bool collided = false;

        for (entt::entity otherEntity : collisionView)
        {
            if (entity == otherEntity)
                continue;

            auto [e2Transform, e2Box] = collisionView.get(otherEntity);

            CollisionBox box1 = collisionBox;
            CollisionBox box2 = e2Box;

            box1.position += transform.position;
            box2.position += e2Transform.position;

            if (box1.CollidesWith(&box2))
                collided = true;
        }

        glm::vec3 globalPosition = transform.position + collisionBox.position;
        globalPosition.z = 0.4f;
        collisionShader->Use();
        collisionShader->UniformFloat4("uChangeCol", { 1, 0, 0, 1 });

        if (collided)
            collisionShader->UniformFloat4("uChangeColWith", { 0, 1, 0, 1 });
        else
            collisionShader->UniformFloat4("uChangeColWith", { 1, 0, 0, 1 });

        Renderer::DrawQuadCustomShader(collisionShader, globalPosition, Sprites::squareTileTexture, collisionBox.size);
    }

    Renderer::EndScene();
}

void TestLayer::OnImGuiRender(Timestep delta)
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
    ImGui::DragFloat2("pos##1", (float*)&cameraController->GetPosition());
    if (ImGui::DragFloat("zoom", (float*)&cameraController->GetZoomLevel()))
        cameraController->SetZoomLevel(cameraController->GetZoomLevel());

    ImGui::SeparatorText("Player");
    ImGui::DragFloat3("pos##2", (float*)&registry.get<TransformComponent>(player).position);
    ImGui::Text("moving: %s", registry.get<MoveComponent>(player).isMoving() ? "Yes" : "No");

    ImGui::SeparatorText("Renderer");
    bool renderDepth = Renderer::IsRenderDepth();
    if (ImGui::Checkbox("Render depth", &renderDepth))
        Renderer::SetRenderDepthOnly(renderDepth);

    if (ImGui::TreeNode("Entity render order"))
    {
        for (int i = 0; i < entityRenderOrder.size(); i++)
        {
            auto [transform, name] = registry.get<TransformComponent, NameComponent>(entityRenderOrder[i]);
            ImGui::DragFloat3(name.name.c_str(), (float*)&transform.position);
        }
        ImGui::TreePop();
    }


    ImGui::End();
    ImGui::SetNextWindowBgAlpha(1.0f);

}

void TestLayer::OnRemove()
{
}