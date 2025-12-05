#pragma once
#include <filesystem>

extern std::filesystem::path g_resourcePath;
extern RECT gBackgroundRect;

extern CRITICAL_SECTION g_cs;

extern HINSTANCE hInst;
extern HWND hWnd;
extern bool useLightning, useWaterWheel, useShotgun, useHourglass;
extern float bulletSpeed;