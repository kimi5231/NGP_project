#pragma once
#include <filesystem>

class GameNetwork;
class GameFramework;

extern GameNetwork* g_network;
extern GameFramework* g_framework;

extern CRITICAL_SECTION g_cs;

extern std::filesystem::path g_resourcePath;
extern RECT gBackgroundRect;

extern HINSTANCE hInst;
extern HWND hWnd;
extern bool useLightning, useWaterWheel, useShotgun, useHourglass;
extern float bulletSpeed;

bool CheckTimer(int& currentTime, int targetTime);