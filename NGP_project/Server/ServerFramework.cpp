#include "pch.h"
#include "ServerFramework.h"
#include "Room.h"
#include "Player.h"

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

	// Client ID 생성기 초기화
	_generateClientID = 1;
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
	for (ClientRef client : _clients)
	{
		FD_SET(client->socket, &_readSet);
		FD_SET(client->socket, &_writeSet);
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

		ProcessAccept(clientSocket);
	}

	for (ClientRef client : _clients)
	{
		if (FD_ISSET(client->socket, &_readSet))
		{
			ProcessRecv(client);
		}

		// send가 가능할 때마다 true
		if (FD_ISSET(client->socket, &_writeSet))
		{
			
		}
	}
}

void ServerFramework::ProcessRecv(ClientRef client)
{
	// PacketSize 수신(고정 길이)
	int packetSize{};
	if (recv(client->socket, (char*)&packetSize, sizeof(int), MSG_WAITALL) <= 0)
	{
		ProcessDisconnect(client);
		return;
	}
		
	// Packet 수신(가변 데이터)
	std::vector<char> packet(512);
	if(recv(client->socket, packet.data(), packetSize, MSG_WAITALL) <= 0)
	{
		ProcessDisconnect(client);
		return;
	}

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
		ProcessMovePacket(movePacket);
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

template<class T>
void ServerFramework::Broadcast(PacketID id, const T& packetData)
{
	// Room에 있는 모든 Client에게 Packet 송신
	for (ClientRef client : _clients)
		ProcessSend(id, packetData, client->socket);
}

GameObjectRef ServerFramework::AddObject(ObjectType type)
{
	// Object 생성
	GameObjectRef object = _room->AddObject(type);
	// Packet Data 생성
	S_AddObject_Packet packetData{ object->GetID(), type, object->GetPos() };
	// 새로운 Object가 추가됨을 Room에 있는 모든 Client에게 알림
	Broadcast(S_AddObject, packetData);

	return object;
}

void ServerFramework::RemoveObject(int id)
{
	std::vector<GameObjectRef>& objects = _room->GetObjects();
	for (GameObjectRef object : objects)
	{
		if (object->GetID() == id)
		{
			// Packet Data 생성
			S_RemoveObject_Packet packetData{ object->GetID(), object->GetObjectType() };
			// Object 삭제
			objects.erase(std::find(objects.begin(), objects.end(), object));
			// Object가 삭제됨을 Room에 있는 모든 Client에게 알림
			Broadcast(S_RemoveObject, packetData);

			return;
		}
	}
}

void ServerFramework::ProcessAccept(SOCKET clientSocket)
{
	// 접속한 Client를 나타낼 Player 추가
	GameObjectRef player = AddObject(ObjectType::Player);

	ClientRef newClient = std::make_shared<Client>();
	newClient->id = _generateClientID++;
	newClient->socket = clientSocket;
	newClient->player = std::dynamic_pointer_cast<Player>(player);

	_clients.push_back(newClient);

	std::cout << "Client" << newClient->id << " 접속" << std::endl;

	// 새로 접속한 Client에게 Room에 있는 모든 Object 정보 송신
	std::vector<GameObjectRef>& objects = _room->GetObjects();
	for (GameObjectRef object : objects)
	{
		// AddObject 함수는 Object 생성까지하기 때문에 사용하지 않음
		S_AddObject_Packet packetData{ object->GetID(), object->GetObjectType(), object->GetPos() };
		ProcessSend(S_AddObject, packetData, newClient->socket);
	}
}

void ServerFramework::ProcessDisconnect(ClientRef client)
{
	// 연결 끊긴 Client를 나타내는 Player 제거
	RemoveObject(client->player->GetID());
	
	std::cout << "Client" << client->id << " 연결 끊김" << std::endl;

	// 연결 끊긴 Client 제거
	closesocket(client->socket);
	_clients.erase(std::find(_clients.begin(), _clients.end(), client));
}

void ServerFramework::ProcessMovePacket(C_Move_Packet packet)
{
	std::vector<GameObjectRef>& objects = _room->GetObjects();
	for (GameObjectRef object : objects)
	{
		if (object->GetID() == packet.objectID)
		{
			object->SetPos(packet.pos);
			S_Move_Packet packetData{ object->GetID(), object->GetObjectType(), object->GetPos()};
			
			// 모든 클라이언트에게 알리기
			for (ClientRef client : _clients)
				ProcessSend(S_Move, packetData, client->socket);

			return;
		}
	}
}