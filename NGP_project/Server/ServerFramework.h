#pragma once
class Room;
class Player;

struct Client
{
	int id;
	SOCKET socket;
	PlayerRef player;
};

class ServerFramework
{
public:
	ServerFramework();
	~ServerFramework();

public:
	void Update();
	void ProcessRecv(ClientRef client);

	template <class T>
	void ProcessSend(PacketID id, const T& packetData, SOCKET clientSocket);

	template <class T>
	std::vector<char> CreatePakcet(PacketID id, const T& packetData);

public:
	// Send
	void SendAddObjectPacket(GameObjectRef object);
	void SendRemoveObjectPacket(GameObjectRef object);
	void SendMovePacket(GameObjectRef object);
	void SendUpdateTimerPacket();

	template <class T>
	void Broadcast(PacketID id, const T& packetData);
	
private:
	// Recv
	void ProcessAccept(SOCKET clientSocket);
	void ProcessDisconnect(ClientRef client);
	void ProcessMovePacket(C_Move_Packet packet);
	void ProcessCollisionPacket(C_Collision_Packet packet);

public:
	Room* GetRoom() { return _room; }

private:
	fd_set _readSet{};
	fd_set _writeSet{};

	SOCKET _listenSocket{}; 
	std::vector<ClientRef> _clients;

	int _generateClientID{};

private:
	Room* _room{};
};
