#include "pch.h"

int main(void)
{
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		std::cout << "윈속 초기화 실패" << std::endl;
		return 0;
	}
		
	// listenSocket 생성
	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
	{
		std::cout << "listenSocket 생성 실패" << std::endl;
		return 0;
	}
		
	// bind
	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(7777);
	if (bind(listenSocket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		std::cout << "bind 실패" << std::endl;
		return 0;
	}

	// listen
	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cout << "listen 실패" << std::endl;
		return 0;
	}

	// Non-Blocking Socket 전환
	u_long on = 1;
	if (ioctlsocket(listenSocket, FIONBIO, &on) == SOCKET_ERROR)
	{
		std::cout << "Non-Blocking Socket 전환 실패" << std::endl;
		return 0;
	}

	fd_set readSet, writeSet;
	while (true)
	{
		// socket set 초기화
		FD_ZERO(&readSet);
		FD_ZERO(&writeSet);

		// readSet에 listenSocket 등록
		FD_SET(listenSocket, &readSet);

		// select
		if (select(0, &readSet, &writeSet, NULL, NULL) == SOCKET_ERROR)
		{
			std::cout << "select 실패" << std::endl;
			return 0;
		}

		// listenSocekt accept 확인
		if (FD_ISSET(listenSocket, &readSet))
		{
			// accept
			SOCKET clientSocket;
			sockaddr_in clientAddr;
			int addrLen = sizeof(clientAddr);
			clientSocket = accept(listenSocket, (sockaddr*)&clientAddr, &addrLen);
			if (clientSocket == INVALID_SOCKET)
				break;
		}
	}

	// listenSocket 종료
	closesocket(listenSocket);

	// 윈속 종료
	WSACleanup();

	return 0;
}