#pragma once
#include <fmod.hpp>
#include <filesystem>
#include <string>

class Sound
{
public:
	Sound();
	virtual ~Sound();

	bool Load(const std::filesystem::path& fullPath, FMOD::System* system, SoundType type);
	void Play(bool loop = false);
	void Stop();

private:
	FMOD::Sound* _sound = nullptr;  // FMOD »ç¿îµå °´Ã¼
	FMOD::Channel* _channel = nullptr;  // FMOD Ã¤³Î °´Ã¼
};