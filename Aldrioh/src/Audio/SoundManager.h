#pragma once

struct ma_sound;

class SoundManager
{
public:

	static void Init();
	static void Destroy();

	static void LoadSound(const std::string& name, const std::string& filePath);
	static void Play(const std::string& soundName);
	static void ClearFinishedSounds();

	static void Test();

private: 
	static std::optional<int> TryGetNextPlaybackSlot();
	static void maSoundEndCallback(void* pUserData, ma_sound* pSound);

};