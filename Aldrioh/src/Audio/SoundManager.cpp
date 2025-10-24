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
	std::array<ma_sound, 100> soundArray;
	std::unordered_map<std::string, int> soundMap;
	int counter = 0;
	std::vector<ma_sound> finishedSounds;
};

static SMData* smdata;

void SoundManager::Init()
{
	smdata = new SMData();

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

void SoundManager::LoadSounds()
{
}

void SoundManager::AddSound(const std::string& name, const std::string& filePath)
{
	ma_result result = ma_sound_init_from_file(smdata->engine, filePath.c_str(), 0, NULL, NULL, &smdata->soundArray[smdata->counter]);
	if (result != MA_SUCCESS)
	{
		LOG_CORE_CRITICAL("Audio failed to load, filepath: {}", filePath);
	}
	smdata->soundMap.insert({ name, smdata->counter });
	++smdata->counter;
}

ma_sound soundtest[200];
int counttest = 0;

void SoundManager::Play(const std::string& soundName)
{
	LOG_CORE_INFO("Playing sound!");
	if (smdata->soundMap.find(soundName) != smdata->soundMap.end())
	{
		ma_sound* soundToPlay = &smdata->soundArray[smdata->soundMap[soundName]];
		ma_result result = ma_sound_init_copy(smdata->engine, soundToPlay, 0, 0, &soundtest[counttest]);
		if (result != MA_SUCCESS) {
			LOG_CORE_INFO("WHAT!!");
		}
		ma_sound_start(&soundtest[counttest++]);

	}
	else
	{
		LOG_CORE_INFO("Sound not added before");
	}
}

void SoundManager::ClearFinishedSounds()
{

}

void SoundManager::Test()
{
	ma_resource_manager* rm = ma_engine_get_resource_manager(smdata->engine);

}
