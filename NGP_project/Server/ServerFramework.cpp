#include "pch.h"
#include "ServerFramework.h"

ServerFramework::ServerFramework()
{
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		std::cout << "윈속 초기화 실패" << std::endl;
		return;
	}

	// listenSocket 생성
	_listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_listenSocket == INVALID_SOCKET)
	{
		std::cout << "listenSocket 생성 실패" << std::endl;
		return;
	}

	// bind
	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(7777);
	if (bind(_listenSocket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		std::cout << "bind 실패" << std::endl;
		return;
	}

	// listen
	if (listen(_listenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cout << "listen 실패" << std::endl;
		return;
	}

	// Non-Blocking Socket 전환
	u_long on = 1;
	if (ioctlsocket(_listenSocket, FIONBIO, &on) == SOCKET_ERROR)
	{
		std::cout << "Non-Blocking Socket 전환 실패" << std::endl;
		return;
	}
}

ServerFramework::~ServerFramework()
{
	// listenSocket 종료
	closesocket(_listenSocket);

	// 윈속 종료
	WSACleanup();
}

void ServerFramework::Update()
{
	// socket set 초기화
	FD_ZERO(&_readSet);
	FD_ZERO(&_writeSet);

	// readSet에 listenSocket 등록
	FD_SET(_listenSocket, &_readSet);

	// readSet, writeSet에 clientSocket 등록
	for (SOCKET client : _clientSockets)
	{
		FD_SET(client, &_readSet);
		FD_SET(client, &_writeSet);
	}
	
	// select
	if (select(0, &_readSet, &_writeSet, NULL, NULL) == SOCKET_ERROR)
	{
		std::cout << "select 실패" << std::endl;
		return;
	}

	// listenSocekt accept 확인
	if (FD_ISSET(_listenSocket, &_readSet))
	{
		// accept
		SOCKET clientSocket;
		sockaddr_in clientAddr;
		int addrLen = sizeof(clientAddr);
		clientSocket = accept(_listenSocket, (sockaddr*)&clientAddr, &addrLen);
		if (clientSocket == INVALID_SOCKET)
		{
			std::cout << "clientSocket 생성 실패" << std::endl;
		}

		_clientSockets.push_back(clientSocket);
		std::cout << "Client 접속" << std::endl;
	}

	for (SOCKET client : _clientSockets)
	{
		if (FD_ISSET(client, &_readSet))
		{
			ProcessRecv(client);
		}

		if (FD_ISSET(client, &_writeSet))
		{
			// 추후 ProcessSend 추가
		}
	}
}

void ServerFramework::ProcessRecv(SOCKET clientSocket)
{
	// PacketSize 수신(고정 길이)
	int packetSize{};
	recv(clientSocket, (char*)&packetSize, sizeof(int), MSG_WAITALL);

	//// Packet 수신(가변 데이터)
	//std::vector<char> packet(512);
	//recv(clientSocket, packet.data(), packetSize, MSG_WAITALL);

	//// Header 추출
	//Header header;
	//memcpy(&header, packet.data(), sizeof(Header));

	//// Data 추출
	//switch (header.id)
	//{
	//case C_UpdateObjectState:
	//	break;
	//case C_UpdateDir:
	//	break;
	//case C_Move:
	//	break;
	//case C_Collision:
	//	break;
	//case C_UseItem:
	//	break;
	//case C_StayGame:
	//	break;
	//case C_EndGame:
	//	break;
	//default:
	//	break;
	//}
}