#pragma once
#include <filesystem>

extern std::filesystem::path g_resourcePath;
extern RECT gBackgroundRect;

extern HINSTANCE hInst;
extern HWND hWnd;
extern bool useLightning, useWaterWheel, useShotgun, useHourglass;
extern float bulletSpeed;

bool CheckTimer(float& currentTime, float targetTime);