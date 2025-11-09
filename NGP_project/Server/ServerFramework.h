#pragma once
class ServerFramework
{
public:
	ServerFramework();
	~ServerFramework();

public:
	void Update();
	void ProcessRecv(SOCKET clientSocket);

private:
	fd_set _readSet{};
	fd_set _writeSet{};

	SOCKET _listenSocket;
	std::vector<SOCKET> _clientSockets;
};