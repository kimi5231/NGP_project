#include "pch.h"
#include "GameNetwork.h"

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

	// Non-Blocking Socket 전환
	u_long on = 1;
	if (ioctlsocket(_socket, FIONBIO, &on) == SOCKET_ERROR)
	{
		return;
		// err_quit("Non-Blocking Socket Switch");
	}

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

void GameNetwork::ProcessSend(const std::vector<char>& packet)
{
	int retval;
	int packetSize = static_cast<int>(packet.size());
	char buf[BUFSIZE];

	// 고정 길이
	retval = send(_socket, (char*)&packetSize, sizeof(int), 0);
	if (retval == SOCKET_ERROR)
	{
		// err_display("send()");
		return;
	}

	// 가변 길이
	retval = send(_socket, packet.data(), packetSize, 0);
	if (retval == SOCKET_ERROR)
	{
		// err_display("send()");
		return;
	}
}

void GameNetwork::processRecv()
{
}

template<class T>
std::vector<char> GameNetwork::CreatePacket(C_PacketID id, T& packet)
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
