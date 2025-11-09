#pragma once
class Room;

class ServerFramework
{
public:
	ServerFramework();
	~ServerFramework();

public:
	void Update();
	void ProcessRecv(SOCKET clientSocket);

public:
	Room* GetRoom() { return _room; }

private:
	fd_set _readSet{};
	fd_set _writeSet{};

	SOCKET _listenSocket;
	std::vector<SOCKET> _clientSockets;

private:
	Room* _room;
};