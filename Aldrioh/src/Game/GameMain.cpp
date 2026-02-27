#include <pch.h>
#include <Core/EntryPoint.h>
#include <Game.h>
#include "GlobalLayers.h"
#include "GlobalResource.h"
#include "Level/Item/ItemTypes.h"

Layer* EntryPoint::GameMain()
{
	Game& game = Game::Instance();

	GR::gr = std::make_unique<GlobalResource>();

	ItemTypes::Init(GR::gr->itemRegistry);

	GlobalLayers::mainMenu = new MainMenuLayer("Main Menu Layer");
	GlobalLayers::game = nullptr;
	GlobalLayers::pauseMenu = new PauseMenuLayer("Pause Menu Layer");
	GlobalLayers::gameOver = new GameOverLayer("Game Over Layer");

	return GlobalLayers::mainMenu;
}