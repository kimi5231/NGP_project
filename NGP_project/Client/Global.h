#pragma once
#include <filesystem>

extern CRITICAL_SECTION g_send_cs;
extern CRITICAL_SECTION g_recv_cs;

extern std::filesystem::path g_resourcePath;
extern RECT gBackgroundRect;

extern HINSTANCE hInst;
extern HWND hWnd;
extern bool useLightning, useWaterWheel, useShotgun, useHourglass;
extern float bulletSpeed;

bool CheckTimer(int& currentTime, int targetTime);