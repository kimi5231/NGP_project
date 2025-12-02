#include "pch.h"
#include "ServerFramework.h"
#include "Room.h"
#include "Global.h"
#include "TimeManager.h"

DWORD WINAPI ProcessGameData(LPVOID client)
{
	GET_SINGLE(TimeManager)->Init();

	while (true)
	{
		GET_SINGLE(TimeManager)->Tick(60.0f);
		g_framework->GetRoom()->Update();
	}
	
	return 0;
}

int main(void)
{	
	// ServerFramework 생성
	g_framework = new ServerFramework();

	InitializeCriticalSection(&g_sendCS);
	InitializeCriticalSection(&g_objectCS);

	// 로직 처리 Thread 생성
	CreateThread(NULL, 0, ProcessGameData, NULL, 0, nullptr);

	while (true)
	{
		g_framework->Update();
	}

	DeleteCriticalSection(&g_sendCS);
	DeleteCriticalSection(&g_objectCS);

	return 0;
}