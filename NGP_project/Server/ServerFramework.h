#pragma once
class Room;
class Player;

struct Client
{
	int id;
	SOCKET socket;
	PlayerRef player;
};

template <class T>
struct SendEvent
{
	bool isComplete = false;
	bool isBroadcast;
	SOCKET clientSocket;
	PacketID packetID;
	T packetData;
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
	void SendAddObjectPacket(GameObjectRef object, bool broadcast, SOCKET client = 0);
	void SendRemoveObjectPacket(GameObjectRef object, bool broadcast, SOCKET client = 0);
	void SendMovePacket(GameObjectRef object, bool broadcast, SOCKET client = 0);
	void SendUpdateTimerPacket(GameObjectRef object, bool broadcast, SOCKET client = 0);

	template <class T>
	void Broadcast(PacketID id, const T& packetData);
	
private:
	// Recv
	void ProcessAccept(SOCKET clientSocket);
	void ProcessDisconnect(ClientRef client);
	void ProcessMovePacket(C_Move_Packet packet);
	void ProcessCreateProjectilePacket(C_CreateProjectile_Packet packet);
	void ProcessCollisionPacket(C_Collision_Packet packet);

public:
	Room* GetRoom() { return _room; }

	void AddRemoveObject(GameObjectRef object) { _removeObjects.push_back(object); }

private:
	fd_set _readSet{};
	fd_set _writeSet{};

	SOCKET _listenSocket{}; 
	std::vector<ClientRef> _clients;

	int _generateClientID{};

private:
	Room* _room{};
	std::vector<GameObjectRef> _removeObjects;
	std::vector<EventType> _sendEvents;
};