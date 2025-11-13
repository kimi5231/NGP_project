#include "pch.h"
#include "SoundManager.h"

SoundManager::~SoundManager()
{
    Clear();
}

void SoundManager::Init(HWND hwnd, const std::filesystem::path& resourcePath)
{
    _hwnd = hwnd;
    _resourcePath = resourcePath;

    // FMOD 시스템 초기화
    FMOD_RESULT result = FMOD::System_Create(&_system);
    if (result != FMOD_OK)
    {
        ::MessageBox(hwnd, L"FMOD system creation failed", L"", MB_OK);
        return;
    }

    result = _system->init(512, FMOD_INIT_NORMAL, nullptr);
    if (result != FMOD_OK)
    {
        ::MessageBox(hwnd, L"FMOD system initialization failed", L"", MB_OK);
        return;
    }
}

void SoundManager::Clear()
{
    StopAll();

    for (auto& pair : _sounds)
        delete pair.second;

    _sounds.clear();

    if (_system)
    {
        _system->release();
        _system = nullptr;
    }
}

Sound* SoundManager::LoadSound(const std::wstring& key, const std::wstring& relativePath, SoundType type)
{
    auto iter = _sounds.find(key);
    if (iter != _sounds.end())
        return iter->second;

    std::filesystem::path fullPath = _resourcePath / relativePath;

    Sound* sound = new Sound();
    if (!sound->Load(fullPath, _system, type))
    {
        ::MessageBox(_hwnd, L"Sound load failed", fullPath.c_str(), MB_OK);
        delete sound;
        return nullptr;
    }

    _sounds[key] = sound;
    return sound;
}

Sound* SoundManager::GetSound(const std::wstring& key)
{
    auto iter = _sounds.find(key);
    if (iter != _sounds.end())
        return iter->second;
    return nullptr;
}

void SoundManager::Play(const std::wstring& key, bool loop)
{
    Sound* sound = GetSound(key);
    if (sound)
        sound->Play(loop);
    else
        ::MessageBox(_hwnd, (L"Sound not found: " + key).c_str(), L"", MB_OK);
}

void SoundManager::Stop(const std::wstring& key)
{
    Sound* sound = GetSound(key);
    if (sound)
        sound->Stop();
}

void SoundManager::StopAll()
{
    for (auto& pair : _sounds)
        pair.second->Stop();
}