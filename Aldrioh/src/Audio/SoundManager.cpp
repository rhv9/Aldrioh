#include <pch.h>
#include "SoundManager.h"
#include <miniaudio.h>
#include <mutex>

struct Sound
{
	ma_sound sound;
};

struct SMData
{
	ma_engine* engine;
	constexpr static int LOADED_SOUND_ARRAY_SIZE = 100;
	std::array<ma_sound, LOADED_SOUND_ARRAY_SIZE> loadedSoundArray;
	std::unordered_map<std::string, int> soundNameMap;
	int loadedCounter = 0;

	constexpr static int PLAYING_SIZE = 200;
	ma_sound playingSounds[PLAYING_SIZE];

	std::vector<int> availablePlayback;
	// Finished slots get cleared every update and added back to playback
	std::vector<int> finishedSlots; 
	
	std::mutex availablePlayback_mutex;
};

static SMData* smdata;

void SoundManager::Init()
{
	smdata = new SMData();
	smdata->availablePlayback.reserve(SMData::PLAYING_SIZE);
	smdata->finishedSlots.reserve(SMData::PLAYING_SIZE);

	for (int i = SMData::PLAYING_SIZE - 1; i > 0; --i)
		smdata->availablePlayback.push_back(i);

	ma_result result;
	smdata->engine = (ma_engine*)malloc(sizeof(*smdata->engine));

	result = ma_engine_init(NULL, smdata->engine);
	if (result != MA_SUCCESS)
		LOG_CORE_CRITICAL("Failed to initialize SoundManager");
}

void SoundManager::Destroy()
{
	ma_engine_uninit(smdata->engine);
	delete smdata->engine;
	delete smdata;
}

void SoundManager::LoadSound(const std::string& name, const std::string& filePath)
{
	ma_result result = ma_sound_init_from_file(smdata->engine, filePath.c_str(), 0, NULL, NULL, &smdata->loadedSoundArray[smdata->loadedCounter]);
	if (result != MA_SUCCESS)
		LOG_CORE_CRITICAL("Audio failed to load, filepath: {}", filePath);

	smdata->soundNameMap.insert({ name, smdata->loadedCounter });
	++smdata->loadedCounter;
}

void SoundManager::Play(const std::string& soundName)
{
	if (smdata->soundNameMap.find(soundName) != smdata->soundNameMap.end())
	{
		ma_sound* soundToPlay = &smdata->loadedSoundArray[smdata->soundNameMap[soundName]];

		std::optional<int> optionalval = TryGetNextPlaybackSlot();
		if (!optionalval.has_value())
		{
			LOG_CORE_INFO("No available slot to play!");
			return;
		}
		int slot = optionalval.value();
		ma_sound* sound = &smdata->playingSounds[slot];
		ma_result result = ma_sound_init_copy(smdata->engine, soundToPlay, 0, 0, sound);
		if (result != MA_SUCCESS) // TODO fix
			LOG_CORE_INFO("Failed to create sound");

		ma_sound_set_end_callback(sound, SoundManager::maSoundEndCallback, NULL);
		ma_sound_start(sound);
	}
	else
		LOG_CORE_INFO("Sound not added before");
}

void SoundManager::ClearFinishedSounds()
{
	if (!smdata->finishedSlots.empty())
	{
		std::lock_guard<std::mutex> lock{ smdata->availablePlayback_mutex };
		for (int slot : smdata->finishedSlots)
		{
			ma_sound_uninit(&smdata->playingSounds[slot]);
			smdata->availablePlayback.push_back(slot);
		}

		smdata->finishedSlots.clear();
	}
}

std::optional<int> SoundManager::TryGetNextPlaybackSlot()
{
	std::lock_guard<std::mutex> lock{ smdata->availablePlayback_mutex };
	if (!smdata->availablePlayback.empty())
	{
		int val = smdata->availablePlayback[smdata->availablePlayback.size() - 1];
		smdata->availablePlayback.pop_back();
		return val;
	}
	return std::optional<int>{};
}

void SoundManager::maSoundEndCallback(void* pUserData, ma_sound* pSound)
{
	uint32_t size = pSound - &smdata->playingSounds[0];

	// Add to clear slot
	std::lock_guard<std::mutex> lock{ smdata->availablePlayback_mutex };
	smdata->finishedSlots.push_back(size);

}

void SoundManager::Test()
{
	ma_resource_manager* rm = ma_engine_get_resource_manager(smdata->engine);
}
