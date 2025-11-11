#pragma once
#include <filesystem>

extern std::filesystem::path g_resourcePath;
extern RECT gBackgroundRect;

extern HINSTANCE hInst;
extern HWND hWnd;

bool CheckTimer(float& currentTime, float targetTime);