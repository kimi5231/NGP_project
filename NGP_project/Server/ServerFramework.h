#pragma once
#include "GameObject.h"

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
	void SenUpdateObjectStatePacket(GameObjectRef object, bool broadcast, SOCKET client = 0);
	void SendMovePacket(GameObjectRef object, bool broadcast, SOCKET client = 0);
	void SendUpdateTimerPacket(GameObjectRef object, bool broadcast, SOCKET client = 0);
	void SendUpdateTimerPacket(bool broadcast, SOCKET client = 0);
	void SendGetItemPacket(ItemRef item, PlayerRef player);
	void SendItemUseResultPacket(PlayerRef player, bool result);
	void SendEndGamePacket(bool broadcast, SOCKET client = 0);

	template <class T>
	void Broadcast(PacketID id, const T& packetData);
	
private:
	// Recv
	void ProcessAccept(SOCKET clientSocket);
	void ProcessDisconnect(ClientRef client);
	void ProcessMovePacket(C_Move_Packet packet);
	void ProcessCreateProjectilePacket(C_CreateProjectile_Packet packet);
	void ProcessUseItemPacket(C_UseItem_Packet packet);

public:
	Room* GetRoom() { return _room; }

	void AddRemoveObject(GameObjectRef object) { _removeObjects[object->GetID()] = object; }

private:
	fd_set _readSet{};
	fd_set _writeSet{};

	SOCKET _listenSocket{}; 
	std::vector<ClientRef> _clients;
	std::vector<ClientRef> _removeClients;

	int _generateClientID{};

private:
	Room* _room{};
	std::unordered_map<int, GameObjectRef> _removeObjects;
	std::vector<EventType> _sendEvents;
};