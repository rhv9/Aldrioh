#pragma once

struct ma_sound;

// If modifying SoundCategory, also change SMData::soundGroups size.
// There has to be atleast MASTER
enum class SoundCategory : uint8_t
{
	MASTER = 0,
	SFX,
	BACKGROUND,
	MENU,
	OTHER,
	DO_NOT_PUT_UNDER__FOR_COUNTING,
};

struct PlayingSoundID
{
	uint16_t slot;
	uint16_t id;

	bool operator==(const PlayingSoundID& other) const
	{
		return slot == other.slot && id == other.id;
	}
};

constexpr PlayingSoundID SOUNDID_NULL{ -1, -1 };
using song_id_t = uint32_t;

class SoundManager
{
public:

	static void Init();
	static void Destroy();

	static void LoadSound(SoundCategory soundCategory, const std::string& name, const std::string& filePath, float volume = 1);
	static void LoadSound(SoundCategory soundCategory, const song_id_t uniqueId, const std::string& filePath, float volume = 1);

	static PlayingSoundID Play(const std::string& soundName);
	// TODO: Optimize uniqueId Right now converting to string
	static PlayingSoundID Play(const song_id_t uniqueId);

	static void RecycleFinishedSounds();

	static void SetVolume(SoundCategory soundCategory, float normalized);
	static float GetVolume(SoundCategory soundCategory);
	static void Test();

	static PlayingSoundID PlayLooping(const std::string& soundName);
	static PlayingSoundID PlayLooping(const song_id_t uniqueId);

	static void Stop(const PlayingSoundID soundId);

private: 
	static std::optional<int> TryGetNextPlaybackSlot();
	static void maSoundEndCallback(void* pUserData, ma_sound* pSound);

};