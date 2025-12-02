#include "pch.h"
#include "Global.h"

ServerFramework* g_framework = nullptr;

CRITICAL_SECTION g_sendCS;

std::random_device g_rd;
std::mt19937 gen(g_rd());
