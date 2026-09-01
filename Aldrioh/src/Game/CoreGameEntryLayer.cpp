#include <pch.h>
#include "CoreGameEntryLayer.h"

#include <Game.h>
#include <Game/GlobalLayers.h>
#include "GameSound.h"

void LoadSounds()
{
	SoundManager::LoadSound(SoundCategory::SFX, "sfx", "assets/audio/sfx_exp_long4.wav");
	SoundManager::LoadSound(SoundCategory::SFX, "player_shoot", "assets/audio/General\ Sounds/High\ Pitched\ Sounds/sfx_sounds_high3.wav", 0.5f);
	SoundManager::LoadSound(SoundCategory::SFX, "bullet_impact", "assets/audio/General\ Sounds/Impacts/sfx_sounds_impact1.wav", 0.5f);
	SoundManager::LoadSound(SoundCategory::SFX, GameSound::exp_gain, "assets/audio/General\ Sounds/Coins/sfx_coin_double5.wav", 0.5f);

	SoundManager::LoadSound(SoundCategory::BACKGROUND, GameSound::background_theme, "assets/audio/background/first_background.wav");
}

void CoreGameEntryLayer::OnBegin()
{
	Game& game = Game::Instance();
	game.GetLayerStack().QueuePopLayer(this);
	game.GetLayerStack().QueuePushLayer(GlobalLayers::menuBackground);
	game.GetLayerStack().QueuePushLayer(GlobalLayers::mainMenu);

	LoadSounds();

	EntityTypes::InitGlobal();
}
