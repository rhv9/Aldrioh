#pragma once
#include "GameLayer.h"
#include "LevelEditor/LevelEditorLayer.h"
#include "Menu/MainMenuLayer.h"
#include "Menu/PauseMenuLayer.h"
#include "Menu/GameOverLayer.h"

#ifdef GLOBAL_LAYERS_CPP
#define GLOBAL_LAYERS_EXTERN
#else
#define GLOBAL_LAYERS_EXTERN extern
#endif

namespace GlobalLayers {

	GLOBAL_LAYERS_EXTERN GameLayer* game;
	GLOBAL_LAYERS_EXTERN MainMenuLayer* mainMenu;
	GLOBAL_LAYERS_EXTERN PauseMenuLayer* pauseMenu;
	GLOBAL_LAYERS_EXTERN LevelEditorLayer* levelEditor;
	GLOBAL_LAYERS_EXTERN GameOverLayer* gameOver;

}