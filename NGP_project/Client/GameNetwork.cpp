#include "pch.h"
#include "GameNetwork.h"
#include "GameScene.h"
#include "Player.h"
#include "NormalMonster.h"
#include "TankMonster.h"
#include "BomberMonster.h"
#include "RespawnMonster.h"
#include "ObstacleMonster.h"
#include "Projectile.h"

// 수영 데스크탑
//char* SERVERIP = (char*)"61.255.49.141";
// 미나 데스크탑
//char* SERVERIP = (char*)"192.168.35.52";	
// 루프백
char* SERVERIP = (char*)"127.0.0.1";

#define SERVERPORT 7777
#define BUFSIZE 512

GameNetwork::GameNetwork()
{
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	// 소켓 생성
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == INVALID_SOCKET)
		return;
		// err_quit("socket()");	// 지금은 서버와 접속 안 돼도 클라 실행되도록 되어있음

	// connect()
	int retval;
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(_socket, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
		return;
		// err_quit("connect()");

}

GameNetwork::~GameNetwork()
{
	// 소켓 닫기
	closesocket(_socket);

	// 윈속 종료
	WSACleanup();
}

void GameNetwork::Update()
{
	// socket set 초기화
	FD_ZERO(&_readSet);
	FD_ZERO(&_writeSet);

	// readSet, wirteSet에 socket 등록
	FD_SET(_socket, &_readSet);
	FD_SET(_socket, &_writeSet);

	// select
	if (select(0, &_readSet, &_writeSet, NULL, NULL) == SOCKET_ERROR)
	{
		//err_display("select");
		return;
	}

	if (FD_ISSET(_socket, &_readSet))
	{
		ProcessRecv();
	}

	if (FD_ISSET(_socket, &_writeSet))
	{
		
	}
}

template<class T>
void GameNetwork::ProcessSend(PacketID id, const T& packet)
{
	// 패킷 생성
	std::vector<char> sendPacket = CreatePacket(id, packet);

	int retval;
	int packetSize = static_cast<int>(sendPacket.size());

	// 고정 길이 데이터 전송
	retval = send(_socket, (char*)&packetSize, sizeof(int), 0);
	if (retval == SOCKET_ERROR)
	{
		// err_display("send()");
		return;
	}

	// 가변 길이
	retval = send(_socket, sendPacket.data(), packetSize, 0);
	if (retval == SOCKET_ERROR)
	{
		// err_display("send()");
		return;
	}
}

void GameNetwork::ProcessRecv()
{
	// PacketSize 수신(고정 길이)
	int packetSize{};
	recv(_socket, (char*)&packetSize, sizeof(int), MSG_WAITALL);

	// Packet 수신(가변 데이터)
	std::vector<char> packet(BUFSIZE);
	recv(_socket, packet.data(), packetSize, MSG_WAITALL);

	// Header 추출
	Header header;
	memcpy(&header, packet.data(), sizeof(Header));

	// Data 추출
	switch (header.id)
	{
	case S_AddObject:
		S_AddObject_Packet addObjectPacket;
		memcpy(&addObjectPacket, packet.data() + sizeof(Header), sizeof(S_AddObject_Packet));
		RecvAddObject(addObjectPacket);
		break;
	case S_RemoveObject:
		S_RemoveObject_Packet removeObjectPacket;
		memcpy(&removeObjectPacket, packet.data() + sizeof(Header), sizeof(S_RemoveObject_Packet));
		RecvRemoveObject(removeObjectPacket);
		break;
	case S_UpdateObjectState:	
		break;
	case S_UpdateDir:
		S_UpdateDir_Packet dirPacket;
		memcpy(&dirPacket, packet.data() + sizeof(Header), sizeof(S_UpdateDir_Packet));
		RecvUpdateDir(dirPacket);
		break;
	case S_Move:	// Player의 pos, dir, state 다 받음
		S_Move_Packet movePacket;
		memcpy(&movePacket, packet.data() + sizeof(Header), sizeof(S_Move_Packet));
		RecvMove(movePacket);
		break;
	case S_ChangeNextStage:
		break;
	case S_CollisionResult:
		break;
	case S_MonsterDamaged:
		break;
	case S_ItemUseResult:
		break;
	case S_UpdateTimer:
		S_UpdateTimer_Packet updateTimerPacket;
		memcpy(&updateTimerPacket, packet.data() + sizeof(Header), sizeof(S_UpdateTimer_Packet));
		RecvUpdateTimer(updateTimerPacket);
	}
}

template<class T>
std::vector<char> GameNetwork::CreatePacket(PacketID id, T& packet)
{
	std::vector<char> retPacket;

	// 헤더
	Header header;
	header.id = id;
	header.dataSize = sizeof(T);

	// 전체 패킷 크기
	int totalSize = sizeof(Header) + sizeof(T);
	retPacket.resize(totalSize);

	// 패킷 넣기
	memcpy(retPacket.data(), &header, sizeof(Header));
	memcpy(retPacket.data() + sizeof(Header), &packet, sizeof(packet));

	return retPacket;
}

void GameNetwork::SendMovePacket(int id, ObjectType type, Vertex pos, Dir dir, ObjectState state)
{
	C_Move_Packet packet;
	packet.objectID = id;
	packet.type = type;
	packet.pos = pos;
	packet.dir = dir;
	packet.state = state;

	ProcessSend(PacketID::C_Move, packet);
}

void GameNetwork::SendCreateProjectilePacket(int id, Vertex pos, Dir dir)
{
	C_CreateProjectile_Packet packet;
	packet.objectID = id;
	packet.type = ObjectType::Bullet;
	packet.pos = pos;
	packet.dir = dir;

	ProcessSend(PacketID::C_CreateProjectile, packet);
}

void GameNetwork::SendCollisionPacket(CollisionType c_type, int id1, ObjectType type1, Vertex pos1, int id2, ObjectType type2, Vertex pos2)
{
	C_Collision_Packet packet;
	packet.collisionType = c_type;
	packet.objectID1 = id1;
	packet.type1 = type1;
	packet.pos1 = pos1;
	packet.objectID2 = id2;
	packet.type2 = type2;
	packet.pos2 = pos2;

	ProcessSend(PacketID::C_Collision, packet);
}

void GameNetwork::SendUseItemPacket(int id, ObjectType itemType)
{
	C_UseItem_Packet packet;
	packet.objectID = id;
	packet.itemType = itemType;

	ProcessSend(PacketID::C_UseItem, packet);
}

void GameNetwork::SendStayGamePacket(int id)
{
	C_StayGame_Packet packet;
	packet.objectID = id;

	ProcessSend(PacketID::C_StayGame, packet);
}

void GameNetwork::SendEndGamePacket(int id)
{
	C_EndGame_Packet packet;
	packet.objectID = id;

	ProcessSend(PacketID::C_EndGame, packet);
}

void GameNetwork::RecvAddObject(S_AddObject_Packet addObjectPacket)
{
	switch (addObjectPacket.type)
	{
	case ObjectType::Player:
	{
		PlayerRef player = std::make_shared<Player>();
		player->SetObjectType(ObjectType::Player);
		player->SetPos(addObjectPacket.pos);
		_gameScene->AddPlayer(addObjectPacket.objectID, player);
	}
	break;

	case ObjectType::Bullet:
	{
		ProjectileRef projectile = std::make_shared<Projectile>();
		projectile->SetObjectType(ObjectType::Bullet);
		projectile->SetPos(addObjectPacket.pos);
		_gameScene->AddObject(addObjectPacket.objectID, projectile);
	}
	break;

	case ObjectType::NormalMonster:
	{
		MonsterRef monster = std::make_shared<NormalMonster>();
		monster->SetObjectType(ObjectType::NormalMonster);
		monster->SetPos(addObjectPacket.pos);
		_gameScene->AddMonster(addObjectPacket.objectID, monster);
	}
	break;

	case ObjectType::TankMonster:
	{
		MonsterRef monster = std::make_shared<TankMonster>();
		monster->SetObjectType(ObjectType::TankMonster);
		monster->SetPos(addObjectPacket.pos);
		_gameScene->AddMonster(addObjectPacket.objectID, monster);
	}
	break;

	case ObjectType::BomberMonster:
	{
		MonsterRef monster = std::make_shared<BomberMonster>();
		monster->SetObjectType(ObjectType::BomberMonster);
		monster->SetPos(addObjectPacket.pos);
		_gameScene->AddMonster(addObjectPacket.objectID, monster);
	}
	break;

	case ObjectType::RespawnMonster:
	{
		MonsterRef monster = std::make_shared<RespawnMonster>();
		monster->SetObjectType(ObjectType::RespawnMonster);
		monster->SetPos(addObjectPacket.pos);
		_gameScene->AddMonster(addObjectPacket.objectID, monster);
	}
	break;

	case ObjectType::ObstacleMonster:
	{
		MonsterRef monster = std::make_shared<ObstacleMonster>();
		monster->SetObjectType(ObjectType::ObstacleMonster);
		monster->SetPos(addObjectPacket.pos);
		_gameScene->AddMonster(addObjectPacket.objectID, monster);
	}
	break;
	}
}

void GameNetwork::RecvRemoveObject(S_RemoveObject_Packet removeObjectPacket)
{
	switch (removeObjectPacket.type)
	{
	case ObjectType::Player:
		_gameScene->RemovePlayer(removeObjectPacket.objectID);
		break;

	case ObjectType::NormalMonster:
	case ObjectType::TankMonster:
	case ObjectType::BomberMonster:
	case ObjectType::RespawnMonster:
	case ObjectType::ObstacleMonster:
		_gameScene->RemoveMonster(removeObjectPacket.objectID);
		break;

	case ObjectType::Bomb:
	case ObjectType::Bullet:
	case ObjectType::Item:
	case ObjectType::Obstacle:
		_gameScene->RemoveObject(removeObjectPacket.objectID);
		break;
	}
}

void GameNetwork::RecvUpdateObjectState(S_UpdateObjectState_Packet updateObjectStatePacket)
{
}

void GameNetwork::RecvUpdateDir(S_UpdateDir_Packet updateDirPacket)
{
}

void GameNetwork::RecvMove(S_Move_Packet movePacket)
{
	switch (movePacket.type)
	{
	case ObjectType::Player:
		_gameScene->SyncPlayer(movePacket.objectID, movePacket.pos, movePacket.dir, movePacket.state);
		break;
	case ObjectType::NormalMonster:
	case ObjectType::TankMonster:
	case ObjectType::BomberMonster:
	case ObjectType::RespawnMonster:
	case ObjectType::ObstacleMonster:
		_gameScene->SyncMonster(movePacket.objectID, movePacket.pos, movePacket.dir, movePacket.state);
		break;
	}
}

void GameNetwork::RecvChangeNextStage(S_ChangeNextStage_Packet changeNextStagePacket)
{
}

void GameNetwork::RecvCollisionResult(S_CollisionResult_Packet collisionResultPacket)
{
}

void GameNetwork::RecvMonsterDamaged(S_MonsterDamaged_Packet monsterDamagedPacket)
{
}

void GameNetwork::RecvItemUseResult(S_ItemUseResult_Packet itemUseResultPacket)
{
}

void GameNetwork::RecvUpdateTimer(S_UpdateTimer_Packet updateTimerPacket)
{
	_gameScene->GetTimerUI().Update(updateTimerPacket.time);
}
