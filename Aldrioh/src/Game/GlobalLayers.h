#pragma once
#include "GameLayer.h"
#include "Menu/MainMenuLayer.h"
#include "Menu/PauseMenuLayer.h"
#include "Menu/GameOverLayer.h"
#include "Game/GameUILayer.h"
#include "Menu/UpgradeMenuLayer.h"
#include "Menu/MenuBackgroundLayer.h"
#include "CoreGameEntryLayer.h"

#ifdef GLOBAL_LAYERS_CPP
#define GLOBAL_LAYERS_EXTERN
#else
#define GLOBAL_LAYERS_EXTERN extern
#endif

namespace GlobalLayers {

	GLOBAL_LAYERS_EXTERN CoreGameEntryLayer* coreGameEntry;

	GLOBAL_LAYERS_EXTERN GameLayer* game;

	GLOBAL_LAYERS_EXTERN MenuBackgroundLayer* menuBackground;
	GLOBAL_LAYERS_EXTERN MainMenuLayer* mainMenu;
	GLOBAL_LAYERS_EXTERN PauseMenuLayer* pauseMenu;
	GLOBAL_LAYERS_EXTERN GameOverLayer* gameOver;

	GLOBAL_LAYERS_EXTERN UpgradeMenuLayer* upgradeMenu;

}