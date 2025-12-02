#pragma once
class ServerFramework;

extern ServerFramework* g_framework;

extern CRITICAL_SECTION g_sendCS;
extern CRITICAL_SECTION g_objectCS;