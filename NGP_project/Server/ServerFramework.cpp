#include "pch.h"
#include "ServerFramework.h"
#include "Room.h"
#include "Player.h"
#include "Item.h"
#include "Global.h"
#include "Item.h"

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

	// 네이글 - 1(OFF, 딜레이 없음), 0(ON, 딜레이 있음)
	int optval = 1;
	setsockopt(_listenSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&optval, sizeof(optval));

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
	delete _room;

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
	if (select(0, &_readSet, &_writeSet, NULL, 0) == SOCKET_ERROR)
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

		// 네이글 - 1(OFF, 딜레이 없음), 0(ON, 딜레이 있음)
		//int optval = 1;
		//setsockopt(clientSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&optval, sizeof(optval));

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
			EnterCriticalSection(&g_sendCS);
			for (auto& sendEvent : _sendEvents)
			{
				std::visit([this, client](auto& event) {
					if (event->isBroadcast)
					{
						Broadcast(event->packetID, event->packetData);
						event->isComplete = true;
						return;
					}
					
					if (client->socket == event->clientSocket)
					{
						ProcessSend(event->packetID, event->packetData, event->clientSocket);
						event->isComplete = true;
					}
				}, sendEvent);
			}
			LeaveCriticalSection(&g_sendCS);

			EnterCriticalSection(&g_sendCS);
			_sendEvents.erase(std::remove_if(_sendEvents.begin(), _sendEvents.end(),
					[](const auto& sendEvent) {
						return std::visit([](const auto& event) {return event->isComplete;}, sendEvent);
					}), _sendEvents.end());
			LeaveCriticalSection(&g_sendCS);
		}
	}

	// 삭제할 Object가 있으면 삭제
	EnterCriticalSection(&g_objectCS);
	for (const auto& item : _removeObjects)
		_room->RemoveObject(item.second->GetObjectType(), item.second->GetID());
	LeaveCriticalSection(&g_objectCS);

	_removeObjects.clear();
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
	case C_Move:
		C_Move_Packet movePacket;
		memcpy(&movePacket, packet.data() + sizeof(Header), sizeof(C_Move_Packet));
		ProcessMovePacket(movePacket);
		break;
	case C_CreateProjectile:
		C_CreateProjectile_Packet createProcjecilePacket;
		memcpy(&createProcjecilePacket, packet.data() + sizeof(Header), sizeof(C_CreateProjectile_Packet));
		ProcessCreateProjectilePacket(createProcjecilePacket);
		break;
	case C_UseItem:
		C_UseItem_Packet useItemPacket;
		memcpy(&useItemPacket, packet.data() + sizeof(Header), sizeof(C_UseItem_Packet));
		ProcessUseItemPacket(useItemPacket);
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

void ServerFramework::SendAddObjectPacket(GameObjectRef object, bool broadcast, SOCKET client)
{
	// Packet Data 생성
	S_AddObject_Packet packetData{ object->GetID(), object->GetObjectType(), object->GetPos(), };

	if (ObjectType::Item == object->GetObjectType()) {
		packetData.itemType = std::dynamic_pointer_cast<Item>(object)->GetItemType();
	}

	// SendEvent 생성
	SendEventRef<S_AddObject_Packet> event = std::make_shared<SendEvent<S_AddObject_Packet>>();
	event->isBroadcast = broadcast;
	event->clientSocket = client;
	event->packetID = S_AddObject;
	event->packetData = packetData;

	EnterCriticalSection(&g_sendCS);
	_sendEvents.push_back(event);
	LeaveCriticalSection(&g_sendCS);
}

void ServerFramework::SendRemoveObjectPacket(GameObjectRef object, bool broadcast, SOCKET client)
{
	// Packet Data 생성
	S_RemoveObject_Packet packetData{ object->GetID(), object->GetObjectType() };
	
	// SendEvent 생성
	SendEventRef<S_RemoveObject_Packet> event = std::make_shared<SendEvent<S_RemoveObject_Packet>>();
	event->isBroadcast = broadcast;
	event->clientSocket = client;
	event->packetID = S_RemoveObject;
	event->packetData = packetData;

	EnterCriticalSection(&g_sendCS);
	_sendEvents.push_back(event);
	LeaveCriticalSection(&g_sendCS);
}

void ServerFramework::SenUpdateObjectStatePacket(GameObjectRef object, bool broadcast, SOCKET client)
{
	S_UpdateObjectState_Packet packetData{ object->GetID(), object->GetObjectType(), object->GetState() };

	// SendEvent 생성
	SendEventRef<S_UpdateObjectState_Packet> event = std::make_shared<SendEvent<S_UpdateObjectState_Packet>>();
	event->isBroadcast = broadcast;
	event->clientSocket = client;
	event->packetID = S_UpdateObjectState;
	event->packetData = packetData;

	EnterCriticalSection(&g_sendCS);
	_sendEvents.push_back(event);
	LeaveCriticalSection(&g_sendCS);
}

void ServerFramework::SendMovePacket(GameObjectRef object, bool broadcast, SOCKET client)
{
	S_Move_Packet packetData{ object->GetID(), object->GetObjectType(), object->GetPos(), object->GetDir(), object->GetState()};
	
	// SendEvent 생성
	SendEventRef<S_Move_Packet> event = std::make_shared<SendEvent<S_Move_Packet>>();
	event->isBroadcast = broadcast;
	event->clientSocket = client;
	event->packetID = S_Move;
	event->packetData = packetData;

	std::cout << "Object " << packetData.objectID << ": Move " << packetData.pos.x << ", " << packetData.pos.y << std::endl;
	
	EnterCriticalSection(&g_sendCS);
	_sendEvents.push_back(event);
	LeaveCriticalSection(&g_sendCS);
}

void ServerFramework::SendUpdateTimerPacket(GameObjectRef object, bool broadcast, SOCKET client)
{
	S_UpdateTimer_Packet packetData{ object->_timer, object->GetID(), object->GetObjectType() };
	// SendEvent 생성
	SendEventRef<S_UpdateTimer_Packet> event = std::make_shared<SendEvent<S_UpdateTimer_Packet>>();
	event->isBroadcast = broadcast;
	event->clientSocket = client;
	event->packetID = S_UpdateTimer;
	event->packetData = packetData;

	EnterCriticalSection(&g_sendCS);
	_sendEvents.push_back(event);
	LeaveCriticalSection(&g_sendCS);
}

void ServerFramework::SendUpdateTimerPacket(bool broadcast, SOCKET client)
{
	S_UpdateTimer_Packet packetData{ _room->GetTimer(), 0, ObjectType::UI };
	// SendEvent 생성
	SendEventRef<S_UpdateTimer_Packet> event = std::make_shared<SendEvent<S_UpdateTimer_Packet>>();
	event->isBroadcast = broadcast;
	event->clientSocket = client;
	event->packetID = S_UpdateTimer;
	event->packetData = packetData;

	EnterCriticalSection(&g_sendCS);
	_sendEvents.push_back(event);
	LeaveCriticalSection(&g_sendCS);
}

void ServerFramework::SendGetItemPacket(ItemRef item, PlayerRef player)
{
	S_GetItem_Packet packetData{ item->GetItemType() };
	// SendEvent 생성
	SendEventRef<S_GetItem_Packet> event = std::make_shared<SendEvent<S_GetItem_Packet>>();
	// 본인한테만 알리면 되므로 false
	event->isBroadcast = false;
	// player와 대응되는 client 찾기
	for (ClientRef& client : _clients)
	{
		if(client->player == player)
			event->clientSocket = client->socket;
	}
	event->packetID = S_GetItem;
	event->packetData = packetData;

	EnterCriticalSection(&g_sendCS);
	_sendEvents.push_back(event);
	LeaveCriticalSection(&g_sendCS);
}

void ServerFramework::SendItemUseResultPacket(PlayerRef player, bool result)
{
	S_ItemUseResult_Packet packetData{ result };
	// SendEvent 생성
	SendEventRef<S_ItemUseResult_Packet> event = std::make_shared<SendEvent<S_ItemUseResult_Packet>>();
	// 본인한테만 알리면 되므로 false
	event->isBroadcast = false;
	// player와 대응되는 client 찾기
	for (ClientRef& client : _clients)
	{
		if (client->player == player)
			event->clientSocket = client->socket;
	}
	event->packetID = S_ItemUseResult;
	event->packetData = packetData;

	EnterCriticalSection(&g_sendCS);
	_sendEvents.push_back(event);
	LeaveCriticalSection(&g_sendCS);
}

void ServerFramework::SendEndGamePacket(bool broadcast, SOCKET client)
{
	S_EndGame_Packet packetData{ true };
	// SendEvent 생성
	SendEventRef<S_EndGame_Packet> event = std::make_shared<SendEvent<S_EndGame_Packet>>();
	event->isBroadcast = broadcast;
	event->clientSocket = client;
	event->packetID = S_EndGame;
	event->packetData = packetData;

	EnterCriticalSection(&g_sendCS);
	_sendEvents.push_back(event);
	LeaveCriticalSection(&g_sendCS);
}

template<class T>
void ServerFramework::Broadcast(PacketID id, const T& packetData)
{
	// Room에 있는 모든 Client에게 Packet 송신
	for (ClientRef client : _clients)
		ProcessSend(id, packetData, client->socket);
}

void ServerFramework::ProcessAccept(SOCKET clientSocket)
{
	// 접속한 Client를 나타낼 Player 추가
	GameObjectRef player = _room->AddObject(ObjectType::Player);

	ClientRef newClient = std::make_shared<Client>();
	newClient->id = _generateClientID++;
	newClient->socket = clientSocket;
	newClient->player = std::dynamic_pointer_cast<Player>(player);

	_clients.push_back(newClient);

	// 새로 접속한 Client에게 자신을 나타낼 Player 정보 송신
	SendAddObjectPacket(player, false, newClient->socket);

	std::cout << "Client" << newClient->id << " 접속" << std::endl;

	// 새로 접속한 Client에게 Room에 있는 Player 정보 송신
	std::unordered_map<int, PlayerRef> players = _room->GetPlayers();
	for (const auto& item : players)
	{
		// 자기 자신 제외
		if (newClient->player->GetID() != item.first)
		{
			SendAddObjectPacket(item.second, false, newClient->socket);
		}
	}

	// 게임 시작 인원이 되면 게임중으로 RoomState 변경
	if (_room->GetPlayerCount() == 2)
		_room->SetRoomState(RoomState::Playing);
}

void ServerFramework::ProcessDisconnect(ClientRef client)
{
	// 연결 끊긴 Client를 나타내는 Player 제거
	_room->RemoveObject(ObjectType::Player, client->player->GetID());
	
	std::cout << "Client" << client->id << " 접속 종료" << std::endl;

	// 연결 끊긴 Client 제거
	closesocket(client->socket);
	_clients.erase(std::find(_clients.begin(), _clients.end(), client));
}

void ServerFramework::ProcessMovePacket(C_Move_Packet packet)
{
	GameObjectRef object = _room->GetObject(packet.type, packet.objectID);

	bool result = object->SetPos(packet.pos);
	object->SetDir(packet.dir);
	object->SetState(packet.state);

	std::cout << "Object " << packet.objectID << ": Move " << packet.pos.x << ", " << packet.pos.y << std::endl;

	// 이동 실패 시, 모두에게 알리기
	if(!result)
		SendMovePacket(object, true);
	else
	{
		// 자신을 제외한 모든 클라이언트에게 알리기
		for (ClientRef client : _clients)
		{
			if (client->player->GetID() != packet.objectID)
				SendMovePacket(object, false, client->socket);
		}
	}
}

void ServerFramework::ProcessCreateProjectilePacket(C_CreateProjectile_Packet packet)
{
	GameObjectRef projectile = _room->AddObject(packet.type, packet.pos, packet.dir);

	std::cout << "Create Object " << projectile->GetID() << std::endl;
}

void ServerFramework::ProcessUseItemPacket(C_UseItem_Packet packet)
{
	// 아이템 사용 결과값
	bool result = false;

	PlayerRef player = dynamic_pointer_cast<Player>(_room->GetObject(packet.objectType, packet.objectID));

	// 사용하려는 아이템이 실제로 있는지 확인
	if (player->GetItem()->GetItemType() == packet.itemType)
	{
		player->UseItem();
		result = true;
	}

	// 사용 요청한 클라이언트에게 결과 알리기
	SendItemUseResultPacket(player, result);
}