#pragma once
#include <unordered_map>
#include <filesystem>
#include <string>
#include <fmod.hpp>
#include "Sound.h"

class SoundManager
{
    DECLARE_SINGLE(SoundManager);

public:
    ~SoundManager();

    void Init(HWND hwnd, const std::filesystem::path& resourcePath);
    void Clear();

    // 사운드 로드 및 접근
    Sound* LoadSound(const std::wstring& key, const std::wstring& relativePath, SoundType type);
    Sound* GetSound(const std::wstring& key);

    // 재생 컨트롤
    void Play(const std::wstring& key, bool loop = false);
    void Stop(const std::wstring& key);
    void StopAll();

    FMOD::System* GetSystem() const { return _system; }

private:
    HWND _hwnd = nullptr;
    std::filesystem::path _resourcePath;
    FMOD::System* _system = nullptr; // FMOD 시스템 객체
    std::unordered_map<std::wstring, Sound*> _sounds; // 로드된 사운드 캐시
};

