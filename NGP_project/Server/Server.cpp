#include "pch.h"
#include "ServerFramework.h"
#include "Room.h"

ServerFramework* g_framework;

DWORD WINAPI ProcessGameData(LPVOID client)
{
	while (true)
	{
		g_framework->GetRoom()->Update();
	}
	
	return 0;
}

int main(void)
{	
	// ServerFramework 积己
	g_framework = new ServerFramework();

	// 肺流 贸府 Thread 积己
	CreateThread(NULL, 0, ProcessGameData, NULL, 0, nullptr);

	while (true)
	{
		g_framework->Update();
	}

	return 0;
}