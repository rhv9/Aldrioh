#pragma once

struct ma_sound;

// If modifying SoundCategory, also change SMData::soundGroups size.
// There has to be atleast MASTER
enum class SoundCategory : uint8_t
{
	MASTER = 0,
	SFX,
	MUSIC,
	MENU,
	OTHER,
};

struct SoundID
{
	uint16_t slot;
	uint16_t id;
};

constexpr SoundID SOUNDID_NULL{ -1, -1 };

class SoundManager
{
public:

	static void Init();
	static void Destroy();

	static void LoadSound(SoundCategory soundCategory, const std::string& name, const std::string& filePath, float volume = 1);
	static SoundID Play(const std::string& soundName);
	static void RecycleFinishedSounds();

	static void SetVolume(SoundCategory soundCategory, float normalized);
	static float GetVolume(SoundCategory soundCategory);
	static void Test();

	static SoundID PlayLooping(const std::string& soundName);
	static void Stop(const SoundID soundId);

private: 
	static std::optional<int> TryGetNextPlaybackSlot();
	static void maSoundEndCallback(void* pUserData, ma_sound* pSound);

};