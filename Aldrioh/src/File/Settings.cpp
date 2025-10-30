#include <pch.h>
#include "Settings.h"
#include <Audio/SoundManager.h>

#include <simdjson.h>

Settings Settings::settings {};

void Settings::LoadOrCreate()
{
	simdjson::ondemand::parser parser;
	simdjson::padded_string json = simdjson::padded_string::load("assets/config/settings.json");
	simdjson::ondemand::document parsed = parser.iterate(json);
	volumeMaster = float(parsed["volume"]["master"]);
	volumeSfx = float(parsed["volume"]["sfx"]);
	volumeMusic = float(parsed["volume"]["music"]);
	volumeMenu = float(parsed["volume"]["menu"]);
}

void Settings::Save()
{
}

void Settings::Apply()
{
	SoundManager::SetVolume(SoundCategory::MASTER, volumeMaster);
	SoundManager::SetVolume(SoundCategory::MUSIC, volumeMusic);
	SoundManager::SetVolume(SoundCategory::SFX, volumeSfx);
	SoundManager::SetVolume(SoundCategory::MENU, volumeMenu);
}

float Settings::GetVolume(SoundCategory soundCategory) const
{
	switch (soundCategory) {
	case SoundCategory::MASTER:
		return volumeMaster;
	case SoundCategory::MENU:
		return volumeMenu;
	case SoundCategory::MUSIC:
		return volumeMusic;
	case SoundCategory::SFX:
		return volumeSfx;
	}
	return -1;
}

bool Settings::SetVolume(SoundCategory soundCategory, float volume)
{
	switch (soundCategory) {
	case SoundCategory::MASTER:
		volumeMaster = volume;
		return true;
	case SoundCategory::MENU:
		volumeMenu = volume;
		return true;
	case SoundCategory::MUSIC:
		volumeMusic = volume;
		return true;
	case SoundCategory::SFX:
		volumeSfx = volume;
		return true;
	}

	return false;
}