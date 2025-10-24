#pragma once

class SoundManager
{
public:

	static void Init();
	static void Destroy();

	static void LoadSounds();
	static void AddSound(const std::string& name, const std::string& filePath);
	static void Play(const std::string& soundName);
	static void ClearFinishedSounds();

	static void Test();
};