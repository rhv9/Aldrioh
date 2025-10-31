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

class SoundManager
{
public:

	static void Init();
	static void Destroy();

	static void LoadSound(SoundCategory soundCategory, const std::string& name, const std::string& filePath, float volume = 1);
	static void Play(const std::string& soundName);
	static void ClearFinishedSounds();

	static void SetVolume(SoundCategory soundCategory, float normalized);
	static float GetVolume(SoundCategory soundCategory);
	static void Test();

private: 
	static std::optional<int> TryGetNextPlaybackSlot();
	static void maSoundEndCallback(void* pUserData, ma_sound* pSound);

};