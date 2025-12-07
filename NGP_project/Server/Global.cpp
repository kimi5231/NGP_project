#include "pch.h"
#include "Global.h"

ServerFramework* g_framework = nullptr;


std::random_device g_rd;
std::mt19937 gen(g_rd());
CRITICAL_SECTION g_sendCS;
CRITICAL_SECTION g_objectCS;
bool g_useMagazine = false;
