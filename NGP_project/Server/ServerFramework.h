#pragma once
class ServerFramework
{
public:
	ServerFramework();
	~ServerFramework();

public:
	void Update();

private:
	fd_set _readSet{};
	fd_set _writeSet{};

	SOCKET _listenSocket;
	std::vector<SOCKET> _clientSockets;
};