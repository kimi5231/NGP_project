#pragma once
class ServerFramework;

extern ServerFramework* g_framework;

extern CRITICAL_SECTION g_objectCS;
extern CRITICAL_SECTION g_sendCS;

extern std::random_device g_rd;
extern std::mt19937 gen;
extern bool g_useMagazine;
