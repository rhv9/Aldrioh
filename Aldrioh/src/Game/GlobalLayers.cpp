#include <pch.h>
#define GLOBAL_LAYERS_CPP
#include "GlobalLayers.h"
#include <Core/LayerInitialiser.h>

Layer* LayerInitialiser::PushFirstLayer()
{
	GlobalLayers::mainMenu = new MainMenuLayer();
	return GlobalLayers::mainMenu;
}

std::vector<Layer*> LayerInitialiser::OtherLayers()
{
	//GlobalLayers::levelEditor = new LevelEditorLayer();
	GlobalLayers::game = new GameLayer();
	GlobalLayers::pauseMenu = new PauseMenuLayer();

	return { GlobalLayers::game, GlobalLayers::pauseMenu};
}