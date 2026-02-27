#include <pch.h>
#include <Core/EntryPoint.h>
#include <Game.h>
#include "GlobalLayers.h"

Layer* EntryPoint::GameMain()
{
	Game& game = Game::Instance();

	GlobalLayers::mainMenu = new MainMenuLayer("Main Menu Layer");
	GlobalLayers::game = nullptr;
	GlobalLayers::pauseMenu = new PauseMenuLayer("Pause Menu Layer");
	GlobalLayers::gameOver = new GameOverLayer("Game Over Layer");

	return GlobalLayers::mainMenu;
}