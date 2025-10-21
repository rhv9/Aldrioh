#pragma once

class SoundManager
{
public:

	static void Init();
	static void Destroy();

	static void LoadSounds();
	static void Play(const std::string& soundName);
};