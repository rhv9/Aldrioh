#include "pch.h"
#include "GameUILayer.h"

#include <Graphics/Renderer.h>
#include <Game/GlobalLayers.h>

#include <Scene/Components.h>
#include <Game/Components/EntityComponents.h>
#include <UI/UIButton.h>
#include <UI/UIImage.h>

#include <Game.h>

#include <Game/GlobalResource.h>

#include "Level/Item/ItemTypes.h"

#include <imgui.h>

GameUILayer::GameUILayer(const std::string& name, Level& level) : Layer(name), level(level) 
{
	expGainCallbackId = level.GetPlayerStats().expGainEventHandler += EVENT_BIND_MEMBER_FUNCTION(GameUILayer::OnExpGainEvent);
	lvlUpCallbackId = level.GetPlayerStats().lvlUpEventHandler += EVENT_BIND_MEMBER_FUNCTION(GameUILayer::OnLevelUpEvent);

	uiManager = std::make_unique<UIManager>();
	// Active UI for game
	{
		uiLevelCountText = new UIText("Level Count", { 2, 4 }, glm::vec2{ 0 });
		uiLevelCountText->SetText("Level: 0");
		uiLevelCountText->SetAnchorPoint(AnchorPoint::LEFT_TOP);
		uiLevelCountText->GetFontStyle().colour = Colour::WHITE;
		uiLevelCountText->SetFontSize(4);
		uiManager->AddUIObject(uiLevelCountText);

		uiPlayerHealthBar = new UIProgressBar("Player Health", { 2, 2 }, { 7, 0.4f });
		uiPlayerHealthBar->SetAnchorPoint(AnchorPoint::CENTER);
		uiPlayerHealthBar->SetBackgroundColour(Colour::GREY);
		uiPlayerHealthBar->SetProgress(1);
		uiManager->AddUIObject(uiPlayerHealthBar);

		expProgressBar = new UIProgressBar("Exp bar", { 0, 0 }, { 50, 2 });
		expProgressBar->SetAnchorPoint(AnchorPoint::LEFT_TOP);
		expProgressBar->SetBackgroundColour(Colour::GREY);
		expProgressBar->SetBarColour(Colour::GREEN);
		expProgressBar->SetScalingBasedWidth(1.0f);
		expProgressBar->SetProgress(0);
		expProgressBar->SetSmoothBar(1);
		uiManager->AddUIObject(expProgressBar);

		uiTimerText = new UIText("Timer", { 0, 4 }, glm::vec2{ 4, 4 });
		uiTimerText->SetText("");
		uiTimerText->SetAnchorPoint(AnchorPoint::CENTER_TOP);
		uiTimerText->GetFontStyle().colour = Colour::WHITE;
		uiTimerText->GetFontStyle().charSpacingPercent = 0.9f;
		uiTimerText->SetFontSize(4);
		uiManager->AddUIObject(uiTimerText);
	}

	std::unique_ptr<Item> hpItem = GR::gr->itemRegistry.CreateInstance(ItemTypes::BaseStat_Hp);
	std::unique_ptr<Item> dmgItem = GR::gr->itemRegistry.CreateInstance(ItemTypes::BaseStat_Dmg);
	std::unique_ptr<Item> luckItem = GR::gr->itemRegistry.CreateInstance(ItemTypes::BaseStat_Luck);

	std::array<Item*, 3> items;
	items[0] = hpItem.get();
	items[1] = dmgItem.get();
	items[2] = luckItem.get();

	// UI for lvl up system
	{
		float w = 70.0f, h = 86.0f;

		lvlBackground = new UIObject("LVL Background", { 0.0f, -4.0f }, { w, h });
		lvlBackground->SetAnchorPoint(AnchorPoint::CENTER);
		lvlBackground->SetBackgroundColour(Colour::BLUE);
		uiManager->AddUIObject(lvlBackground);

		UIText* lvlUpTitle = new UIText("LVL up title", { 0.0f, 4.0f }, { 0.0f, 0.0f });
		lvlUpTitle->SetAnchorPoint(AnchorPoint::CENTER_TOP);
		lvlUpTitle->GetFontStyle().WithSize(4.0f).WithColour(Colour::WHITE).WithCharSpacingPercent(0.85f);
		lvlUpTitle->SetText("<!Level Up!>");
		lvlBackground->AddChild(lvlUpTitle);

		UIText* lvlUpExplanation = new UIText("LVL up explanation", { 0.0f, lvlUpTitle->GetRelativePosY() + lvlUpTitle->GetHeight() + 3.0f }, { 0.0f, 0.0f });
		lvlUpExplanation->SetAnchorPoint(AnchorPoint::CENTER_TOP);
		lvlUpExplanation->GetFontStyle().WithSize(2.0f).WithColour(Colour::WHITE).WithCharSpacingPercent(0.85f);
		lvlUpExplanation->SetText("Choose your upgrade!");
		lvlBackground->AddChild(lvlUpExplanation);

		float lvlUpHeightOffset = lvlUpTitle->GetRelativePosY() + lvlUpTitle->GetHeight();
		float yButtonHeight = 20.0f;
		float bw = w - 6.0f;
		float yOffset = 10.0f;
		float yGapBetween = 2.1f;

		for (int i = 0; i < 3; ++i)
		{
			float yRelativePos = lvlUpHeightOffset + yButtonHeight * i + yOffset + yGapBetween * i;
			lvlupCards[i] = new UIButton(std::format("Option {}", i), { 0.0f, yRelativePos }, { bw, yButtonHeight });
			lvlupCards[i]->SetAnchorPoint(AnchorPoint::CENTER_TOP);
			lvlupCards[i]->SetButtonColour(Colour::BLACK);
			lvlupCards[i]->SetHoverColour(-0.15f);
			lvlupCards[i]->SetOnClickCallback([i](UIButton* button) {
				GlobalLayers::game->GetUILayer()->SelectLvlUpCard(i);
				GlobalLayers::game->GetUILayer()->SetLvlUpUIActive(false);
				GlobalLayers::game->SetShouldUpdate(true);
				});
			lvlBackground->AddChild(lvlupCards[i]);

			float ixp = 2.0f;
			float iyp = 2.0f;
			float imageSize = 5.0f;

			lvlupImages[i] = new UIImage(std::format("Image {}", i), { ixp, iyp }, { imageSize, imageSize });
			lvlupImages[i]->SetAnchorPoint(AnchorPoint::LEFT_TOP);
			lvlupImages[i]->SetSubTexture(Sprites::get(Sprites::null));
			lvlupCards[i]->AddChild(lvlupImages[i]);

			lvlupTitles[i] = new UIText(std::format("Title {}", i), { ixp + imageSize + 1.0f, iyp + 1.0f }, { 0.0f, 0.0f });
			lvlupTitles[i]->SetAnchorPoint(AnchorPoint::LEFT_TOP);
			lvlupTitles[i]->GetFontStyle().WithSize(3.0f).WithColour(Colour::WHITE);
			lvlupTitles[i]->SetText("Abra cadabra");
			lvlupCards[i]->AddChild(lvlupTitles[i]);

			lvlupLittleInfos[i] = new UIText(std::format("Little Info {}", i), { ixp + 1.25f, iyp + 1.25f }, { 0.0f, 0.0f });
			lvlupLittleInfos[i]->SetAnchorPoint(AnchorPoint::RIGHT_TOP);
			lvlupLittleInfos[i]->GetFontStyle().WithSize(2.5f).WithColour(Colour::BLUE).WithCharSpacingPercent(0.8f);
			lvlupLittleInfos[i]->SetText("2->3");
			lvlupCards[i]->AddChild(lvlupLittleInfos[i]);

			float lvlDescriptionStartX = ixp + 1.25f;
			lvlupDescriptions[i] = new UIText(std::format("Description {}", i), { lvlDescriptionStartX, iyp + imageSize + 2.0f }, { 0.0f, 0.0f });
			lvlupDescriptions[i]->SetAnchorPoint(AnchorPoint::LEFT_TOP);
			lvlupDescriptions[i]->GetFontStyle().WithSize(2.0f).WithColour(glm::vec4{ 0.9f, 0.9f, 0.9f, 1.0f });
			lvlupDescriptions[i]->SetTextWrap(true);
			lvlupDescriptions[i]->SetTextWrapMaxWidth(bw - lvlDescriptionStartX * 2);
			lvlupDescriptions[i]->SetText("Increases Item Stat by 10%, this should never show!");
			lvlupCards[i]->AddChild(lvlupDescriptions[i]);
		}
	}

	// Item ui related
	{
		uniqueItemContainer.reserve(10);
		shipModuleItemContainer.reserve(10);
		baseStatItemContainer.reserve(10);

		itemContainer = new UIObject("UI Item main div", { 2.0f, 10.0f }, { 0.0f, 0.0f });
		itemContainer->SetAnchorPoint(AnchorPoint::LEFT_TOP);
		uiManager->AddUIObject(itemContainer);
	}

	SetLvlUpUIActive(false);

}


void GameUILayer::SetLvlUpCardItem(int i, ItemID itemId, const LvlUpInfo& lvlUpInfo, const std::string& littleInfo)
{
	ItemDef& itemDef = GR::gr->itemRegistry.GetItemDef(itemId);
	glm::vec4 backgroundCol = Colour::BLUE;
	switch (itemDef.id.category)
	{
	case ItemCategory::BaseStat:
		backgroundCol = Colour::RGBCol(227, 106, 7);
		break;
	case ItemCategory::ShipModule:
		backgroundCol = Colour::RGBCol(107, 20, 32);
		break;
	case ItemCategory::Unique:
		backgroundCol = Colour::RGBCol(19, 118, 120);
		break;
	}

	lvlupImages[i]->SetSubTexture(Sprites::get(itemDef.spriteId));
	lvlupTitles[i]->SetText(itemDef.name);
	lvlupLittleInfos[i]->SetText(littleInfo);
	lvlupDescriptions[i]->SetText(lvlUpInfo.msg);
	lvlupCards[i]->SetButtonColour(backgroundCol);
}

UIObject* CreateUIItemSlot(float imageSize, float scale)
{
	UIObject* uiObj = new UIObject("UIItem Container", glm::vec2(0), glm::vec2(imageSize * scale));
	uiObj->SetAnchorPoint(AnchorPoint::LEFT_TOP);

	UIImage* uiImage = new UIImage("UIItem Image", glm::vec2(0), glm::vec2(imageSize * scale));
	uiImage->SetAnchorPoint(AnchorPoint::LEFT_TOP);
	uiObj->AddChild(uiImage);

	UIText* uiText = new UIText("UIItem Text", glm::vec2(0), glm::vec2());
	uiText->SetAnchorPoint(AnchorPoint::RIGHT_BOTTOM);
	uiText->SetText("1");
	uiText->SetFontStyle(uiText->GetFontStyle().WithColour(Colour::WHITE).WithSize(1.0f * scale));
	uiObj->AddChild(uiText);

	return uiObj;
}

void GameUILayer::UpdateItemUI(const ModularShipComponent& msc)
{
	constexpr float scale = 1.2f;
	constexpr float imageSize = 4.0f;
	constexpr float widthGap = (imageSize + 0.8f) * scale;
	constexpr float heightGap = (imageSize + 1.0f) * scale;

	for (int i = 0; i < msc.uniqueCount; ++i)
	{
		if (i >= uniqueItemContainer.size())
		{
			UIObject* newUIObj = CreateUIItemSlot(imageSize, scale);
			newUIObj->SetRelativePos({ i * widthGap, 0.0f });
			static_cast<UIText*>(newUIObj->GetFirstChild(UIType::Text))->Disable();
			itemContainer->AddChild(newUIObj);

			uniqueItemContainer.push_back(newUIObj);
		}
		UIObject* uiObj = uniqueItemContainer[i];
		UIImage* uiImage = static_cast<UIImage*>(uiObj->GetFirstChild(UIType::Image));
		uiImage->SetSubTexture(Sprites::get(msc.uniqueItems[i]->cachedSpriteId));

		UIText* uiText = static_cast<UIText*>(uiObj->GetFirstChild(UIType::Text));
		uiText->SetText(std::to_string(msc.uniqueItems[i]->lvl));
	}

	for (int i = 0; i < msc.shipModuleCount; ++i)
	{
		if (i >= shipModuleItemContainer.size())
		{
			UIObject* newUIObj = CreateUIItemSlot(imageSize, scale);
			newUIObj->SetRelativePos({ i * widthGap, heightGap });
			itemContainer->AddChild(newUIObj);

			shipModuleItemContainer.push_back(newUIObj);
		}
		UIObject* uiObj = shipModuleItemContainer[i];
		UIImage* uiImage = static_cast<UIImage*>(uiObj->GetFirstChild(UIType::Image));
		uiImage->SetSubTexture(Sprites::get(msc.shipModuleItems[i]->cachedSpriteId));

		UIText* uiText = static_cast<UIText*>(uiObj->GetFirstChild(UIType::Text));
		uiText->SetText(std::to_string(msc.shipModuleItems[i]->lvl));
	}

	for (int i = 0; i < msc.baseStatCount; ++i)
	{
		if (i >= baseStatItemContainer.size())
		{
			UIObject* newUIObj = CreateUIItemSlot(imageSize, scale);
			newUIObj->SetRelativePos({ i * widthGap, heightGap * 2.0f});
			itemContainer->AddChild(newUIObj);

			baseStatItemContainer.push_back(newUIObj);
		}
		UIObject* uiObj = baseStatItemContainer[i];
		UIImage* uiImage = static_cast<UIImage*>(uiObj->GetFirstChild(UIType::Image));
		uiImage->SetSubTexture(Sprites::get(msc.baseStatItems[i]->cachedSpriteId));

		UIText* uiText = static_cast<UIText*>(uiObj->GetFirstChild(UIType::Text));
		uiText->SetText(std::to_string(msc.baseStatItems[i]->lvl));
	}
}

void GameUILayer::OnUpdate(Timestep delta)
{
	if (GlobalLayers::game == nullptr)
		return;

	if (GlobalLayers::game->IsPlayerDead())
		return;

	// Update health bar
	Level* level = GlobalLayers::game->GetCurrentLevel();
	auto& hc = level->GetPlayer().GetComponent<HealthComponent>();
	uiPlayerHealthBar->SetProgress(hc.health / hc.maxHealth);

	// Update timer bar
	float elapsedTime = level->GetElapsedTime();
	int mins = static_cast<int>(elapsedTime / 60.0f);
	int seconds = static_cast<int>(elapsedTime) % 60;
	std::string timerText = std::format("{}:{:02}", mins, seconds);
	uiTimerText->SetText(timerText);

	uiManager->OnUpdate(delta);
}

void GameUILayer::OnRender(Timestep delta)
{
	// Update player health bar position
	UIProgressBar* uiPlayerHealthBar = GlobalLayers::game->GetUILayer()->GetUIHealthProgressBar();
	auto& playerCameraController = GlobalLayers::game->GetCurrentLevel()->GetPlayerCamera().GetComponent<CameraComponent>().cameraController;
	glm::vec2 playerPos = GlobalLayers::game->GetCurrentLevel()->GetPlayer().GetTransformComponent().CalculateInterpolatePosition(delta);
	glm::vec2 playerCameraPos = playerCameraController->GetPosition();
	glm::vec2 barPos = playerPos - playerCameraPos;
	barPos.y -= 0.7f;

	// TODO this does not need to be calculated every frame
	glm::vec2 uiArea = uiPlayerHealthBar->GetUIManager()->GetUIArea();
	glm::vec2 cameraArea = playerCameraController->GetBounds().GetSize();
	glm::vec2 cameraToUIMapping{ uiArea.x / cameraArea.x, uiArea.y / cameraArea.y };

	barPos.x *= cameraToUIMapping.x;
	barPos.y *= cameraToUIMapping.y;

	uiPlayerHealthBar->SetRelativePos(barPos);

	Renderer::StartUIScene();
	uiManager->OnRender(delta);
	Renderer::EndUIScene();
}

void GameUILayer::OnImGuiRender(Timestep delta)
{
}

void GameUILayer::OnLevelUpEvent(PlayerStatsEventArg& e)
{
	GlobalLayers::game->SetShouldUpdate(false);
	chosenItems = level.GetItemPool().GenerateThreeUniqueItems();

	// logic to update the ui
	for (int i = 0; i < chosenItems.size(); ++i)
	{
		ItemID itemId = chosenItems[i];

		auto& msc = level.GetPlayer().GetComponent<ModularShipComponent>();
		Item* playerItemPtr = msc.GetItem(itemId);
		if (playerItemPtr != nullptr)
		{
			int lvl = playerItemPtr->lvl;
			LvlUpInfo lvlUpInfo = playerItemPtr->LevelUpPretend();
			SetLvlUpCardItem(i, playerItemPtr->id, lvlUpInfo, std::format("{}->{}", lvl, lvl + 1));
		}
		else
		{
			SetLvlUpCardItem(i, itemId, { GR::gr->itemRegistry.GetItemDef(itemId).description }, "New");
		}
	}

	SetLvlUpUIActive(true);
	uiLevelCountText->SetText(std::format("Level: {}", e.level.GetPlayerStats().GetLevelCount()));
}

void GameUILayer::SelectLvlUpCard(int button)
{
	ItemID itemId = chosenItems[button];
	auto& msc = level.GetPlayer().GetComponent<ModularShipComponent>();
	msc.AddItem(itemId);
	level.GetPlayer().GetComponent<StatComponent>().dirty = true;

	UpdateItemUI(msc);
}
void GameUILayer::OnExpGainEvent(PlayerStatsEventArg& e)
{
	expProgressBar->SetProgress(e.level.GetPlayerStats().GetExpPercent());
}


void GameUILayer::SetLvlUpUIActive(bool active)
{
	if (active)
		lvlBackground->Enable();
	else
		lvlBackground->Disable();
}



void GameUILayer::OnTransitionIn()
{
	uiManager->OnTransitionIn();
}

void GameUILayer::OnMouseButtonEvent(MouseButtonEventArg& e)
{
	uiManager->OnMouseButton(e);
	e.isHandled = GlobalLayers::pauseMenu->IsPaused();
}

void GameUILayer::OnMouseMoveEvent(MouseMoveEventArg& e)
{
	uiManager->OnMouseMove(e);
	e.isHandled = GlobalLayers::pauseMenu->IsPaused();
}

void GameUILayer::OnWindowResizeEvent(WindowResizeEventArg& e)
{
	uiManager->OnWindowResize(e);
}
