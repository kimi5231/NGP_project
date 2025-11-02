#include "pch.h"
#include "GameNetwork.h"

char* SERVERIP = (char*)"127.0.0.1";
#define SERVERPORT 7777
#define BUFSIZE 512

GameNetwork::GameNetwork()
{
	// 扩加 檬扁拳
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	// 家南 积己
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		return;

	// connect
	int retval;
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
		return;
}

GameNetwork::~GameNetwork()
{
}
