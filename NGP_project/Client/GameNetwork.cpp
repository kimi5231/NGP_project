#include "pch.h"
#include "GameNetwork.h"

//char* SERVERIP = (char*)"61.255.49.141";
//char* SERVERIP = (char*)"192.168.35.52";
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
		// Move Packet 전송 테스트
		C_Move_Packet movePacket;
		movePacket.objectID = 1;
		movePacket.type = ObjectType::Player;
		movePacket.pos = { 100, 100 };

		ProcessSend(PacketID::C_Move, movePacket);
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
	std::vector<char> packet(BUFSIZ);
	recv(_socket, packet.data(), packetSize, MSG_WAITALL);

	// Header 추출
	Header header;
	memcpy(&header, packet.data(), sizeof(Header));

	// Data 추출
	switch (header.id)
	{
	case S_AddObject:
		S_AddObject_Packet addObjectPacket;
		memcpy(&addObjectPacket, packet.data() + sizeof(Header), sizeof(S_AddObject));
		break;
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
