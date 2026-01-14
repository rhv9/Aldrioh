#include <pch.h>
#define GLOBAL_LAYERS_CPP
#include "GlobalLayers.h"
#include <Core/LayerInitialiser.h>

Layer* LayerInitialiser::PushFirstLayer()
{
	GlobalLayers::mainMenu = new MainMenuLayer("Main Menu Layer");
	return GlobalLayers::mainMenu;
}

std::vector<Layer*> LayerInitialiser::OtherLayers()
{
	//GlobalLayers::levelEditor = new LevelEditorLayer();
	GlobalLayers::game = new GameLayer("Game Layer");
	GlobalLayers::pauseMenu = new PauseMenuLayer("Pause Menu Layer");
	GlobalLayers::gameOver = new GameOverLayer("Game Over Layer");

	return { GlobalLayers::game, GlobalLayers::pauseMenu, GlobalLayers::gameOver };
}