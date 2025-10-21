#include <pch.h>
#include "SoundManager.h"
#include <miniaudio.h>

struct Sound
{
	ma_sound sound;
};

struct SMData
{
	ma_engine* engine;
	std::unordered_map<std::string, Sound> soundMap;
};

static SMData* smdata;

void SoundManager::Init()
{
	smdata = new SMData();
	smdata->soundMap.reserve(100);

	ma_result result;
	smdata->engine = (ma_engine*)malloc(sizeof(*smdata->engine));

	result = ma_engine_init(NULL, smdata->engine);
	if (result != MA_SUCCESS)
	{
		LOG_CORE_CRITICAL("Failed to initialize SoundManager");
		ASSERT(false, "Failed to initialize SoundManager");
	}
}

void SoundManager::Destroy()
{
	ma_engine_uninit(smdata->engine);
	delete smdata->engine;
	delete smdata;
}

Sound AddSound(const std::string& filePath)
{
	ma_sound sound;
	ma_result result = ma_sound_init_from_file(smdata->engine, filePath.c_str(), 0, NULL, NULL, &sound);
	if (result != MA_SUCCESS)
	{
		LOG_CORE_CRITICAL("Audio failed to load, filepath: {}", filePath);
	}
	return Sound(sound);
}

void SoundManager::LoadSounds()
{
	smdata->soundMap.insert({ std::string("sfx"), AddSound("assets/audio/sfx_exp_long4.wav") });
}

void SoundManager::Play(const std::string& soundName)
{
	if (smdata->soundMap.find(soundName) != smdata->soundMap.end())
	{
		ma_sound_start(&smdata->soundMap[soundName].sound);
	}
}
