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
		g_framework->GetRoom()->Update();
		GET_SINGLE(TimeManager)->Tick(60.0);
	}
	
	return 0;
}

int main(void)
{	
	// ServerFramework 积己
	g_framework = new ServerFramework();

	//GET_SINGLE(TimeManager)->Init();

	InitializeCriticalSection(&g_cs);

	// 肺流 贸府 Thread 积己
	CreateThread(NULL, 0, ProcessGameData, NULL, 0, nullptr);

	while (true)
	{
		g_framework->Update();
		//GET_SINGLE(TimeManager)->Update();
	}

	DeleteCriticalSection(&g_cs);

	return 0;
}