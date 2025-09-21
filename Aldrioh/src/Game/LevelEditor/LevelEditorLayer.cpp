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

	callbackMousePressedID = Game::Instance().GetWindow()->MouseButtonPressedEventHandler += [&](MouseButtonPressedEventArg& arg)
		{
			auto view = scene->getRegistry().view<LevelComponent>();

			for (auto e : view)
			{
				auto& lc = view.get<LevelComponent>(e);
				TestLevel* level = (TestLevel*)lc.level;

				glm::vec2 pos = scene->GetMousePosInScene();

				level->SetTile((int)pos.x, (int)pos.y, new TexturedTiles(Sprites::sand_cactus));
			}
		};

	// UI Related
	Renderer::SetUIPixelHeight(100);

	uiManager = new UIManager();
	UIObject* rectangle = new UIObject("Red box", { 0, 0 }, { 20, 80 });
	rectangle->SetBackgroundColour({ 1.0f, 0.0f, 0.0f, 1.0f });
	uiManager->AddUIObject(rectangle);

	UIObject* rectangle2 = new UIObject("Yellow box", { 0, 0 }, { 10, 10 });
	rectangle2->SetBackgroundColour({ 1.0f, 1.0f, 0.0f, 1.0f });
	rectangle->AddChild(rectangle2);

	UIObject* rectangle3 = new UIObject("Blue box", { 0, 0 }, { 2, 2 });
	rectangle3->SetBackgroundColour({ 0.0f, 0.0f, 1.0f, 1.0f });
	rectangle2->AddChild(rectangle3);

	UIText* uiText = new UIText("Some label", { 0, 0 }, { 20, 20 });
	uiText->SetText("Why Hello There!");
	uiText->SetBackgroundColour({ 0.0f, 0.0f, 0.0f, 0.25f });
	uiText->SetFontSize(4);
	rectangle->AddChild(uiText);

	UIImage* uiImage = new UIImage("UI Image", { 10.0f, 0.0f }, { 10.0f, 10.0f });
	uiImage->SetSubTexture(Sprites::get(Sprites::fire));
	rectangle->AddChild(uiImage);

	UIButton* uiButton = new UIButton("UI Button", { 0, 0 }, { 20, 10 });
	uiButton->SetBackgroundColour({ 0.5f, 0.5f, 0.5f, 1.0f });
	uiButton->GetUIText()->SetText("Click Me!");
	uiButton->GetUIText()->SetFontSize(2.5f);

	uiButton->SetOnClickCallback([](UIButton* button)
		{
			LOG_CORE_INFO("This is callback working!!");
			button->SetBackgroundColour({ Math::Random::linearFloat(0, 1), Math::Random::linearFloat(0, 1), Math::Random::linearFloat(0, 1), 1.0f});
		});

	rectangle->AddChild(uiButton);


	rectangle->SetAnchorPoint(AnchorPoint::CENTER);
	rectangle2->SetAnchorPoint(AnchorPoint::RIGHT_TOP);
	rectangle3->SetAnchorPoint(AnchorPoint::CENTER);
	uiText->SetAnchorPoint(AnchorPoint::CENTER);
	uiImage->SetAnchorPoint(AnchorPoint::RIGHT_TOP);
	uiButton->SetAnchorPoint(AnchorPoint::CENTER_BOTTOM);


	LOG_CORE_INFO("SAND SPRITE TEXTURE ID: {}", Sprites::get(Sprites::sand_1)->textureParent->GetTextureId());
	LOG_CORE_INFO("Font texture ID: {}", Font::DEFAULT->GetTexture()->GetTextureId());
}

static glm::vec2 pos{ 0 }, size{ 900, 100 };

void LevelEditorLayer::OnUpdate(Timestep delta)
{
	scene->OnUpdate(delta);
	scene->OnRender(delta);

	Renderer::StartUIScene();

	uiManager->OnUpdate(delta);
	uiManager->OnRender(delta);

	Renderer::UIDrawTexture(Sprites::get(Sprites::fire), { 0.0f, 0.0f }, { 30.0f, 30.0f }, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0);

	Renderer::UIDrawText(Font::DEFAULT, "Hello There!!", { UIData::PIXEL, 0.0f, 0.0f }, 10, glm::vec4{ 0.7f, 0.0f, 0.7f, 1.0f });

	Renderer::EndUIScene();
}


void LevelEditorLayer::OnImGuiRender(Timestep delta)
{
	Game& game = Game::Instance();
	ImGuiIO& io = ImGui::GetIO();

	ImGui::SetNextWindowBgAlpha(0.6f);
	ImGui::Begin("Main Window");

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
		if (ImGui::BeginTabItem("Level Editor"))
		{

			SubTexture* subTexture = Sprites::get(Sprites::animBossRight[1]);
			glm::vec2 uv1 = subTexture->textureCoords.bottomLeft;
			glm::vec2 uv0 = subTexture->textureCoords.topRight;

			ImGui::Image(subTexture->textureParent->GetTextureId(), { (float)256, (float)256 }, { uv0.x, uv0.y }, { uv1.x, uv1.y });

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
