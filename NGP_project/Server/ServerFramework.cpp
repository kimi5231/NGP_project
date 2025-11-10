#include "pch.h"
#include "ServerFramework.h"
#include "Room.h"

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

	// Room 생성
	_room = new Room();
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

		std::cout << "Client 접속" << std::endl;

		_clientSockets.push_back(clientSocket);

		// 접속한 Client를 나타낼 Player 추가
		GameObjectRef player = _room->AddObject(ObjectType::Player);
		// Player 추가 BroadCast
		C_AddObject_Packet packetData{ 1, ObjectType::Player, player->GetPos() };
		for (SOCKET client : _clientSockets)
			ProcessSend(S_AddObject, packetData, client);
	}

	for (SOCKET client : _clientSockets)
	{
		if (FD_ISSET(client, &_readSet))
		{
			ProcessRecv(client);
		}

		// send가 가능할 때마다 true
		if (FD_ISSET(client, &_writeSet))
		{
			
		}
	}
}

void ServerFramework::ProcessRecv(SOCKET clientSocket)
{
	// PacketSize 수신(고정 길이)
	int packetSize{};
	recv(clientSocket, (char*)&packetSize, sizeof(int), MSG_WAITALL);

	// Packet 수신(가변 데이터)
	std::vector<char> packet(512);
	recv(clientSocket, packet.data(), packetSize, MSG_WAITALL);

	// Header 추출
	Header header;
	memcpy(&header, packet.data(), sizeof(Header));

	// Data 추출
	switch (header.id)
	{
	case C_UpdateObjectState:
		break;
	case C_UpdateDir:
		break;
	case C_Move:
		C_Move_Packet movePacket;
		memcpy(&movePacket, packet.data() + sizeof(Header), sizeof(C_Move_Packet));
		break;
	case C_Collision:
		break;
	case C_UseItem:
		break;
	case C_StayGame:
		break;
	case C_EndGame:
		break;
	}
}

template <class T>
void ServerFramework::ProcessSend(PacketID id, const T& packetData, SOCKET clientSocket)
{
	std::vector<char> packet = CreatePakcet(id, packetData);
	int packetSize = sizeof(packet);

	// packetSize 송신(고정 길이)
	send(clientSocket, (char*)&packetSize, sizeof(int), 0);
	// packet 송신(가변 데이터)
	send(clientSocket, packet.data(), packetSize, 0);
}

template<class T>
std::vector<char> ServerFramework::CreatePakcet(PacketID id, const T& packetData)
{
	// Header
	Header header;
	header.id = id;
	header.dataSize = sizeof(packetData);

	// Packet
	std::vector<char> packet(sizeof(Header) + header.dataSize);
	memcpy(packet.data(), &header, sizeof(Header));
	memcpy(packet.data() + sizeof(Header), &packetData, header.dataSize);

	return packet;
}