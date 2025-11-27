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
		GET_SINGLE(TimeManager)->Update();
	}
	
	return 0;
}

int main(void)
{	
	// ServerFramework 생성
	g_framework = new ServerFramework();

	// 로직 처리 Thread 생성
	CreateThread(NULL, 0, ProcessGameData, NULL, 0, nullptr);

	while (true)
	{
		g_framework->Update();
	}

	return 0;
}