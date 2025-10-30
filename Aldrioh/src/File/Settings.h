#pragma once

#include <Audio/SoundManager.h>

struct Settings
{
public:
	static Settings& Get() { return Settings::settings; }
	void LoadOrCreate();
	Settings Copy() { return Settings::settings; }
	void Save();
	void Apply();

	float GetVolume(SoundCategory soundCategory) const;
	bool SetVolume(SoundCategory soundCategory, float volume);

private:
	float volumeMaster = 0.5, volumeSfx = 1, volumeMenu = 1, volumeMusic = 1;
	
	static Settings settings;
	Settings() = default;
};