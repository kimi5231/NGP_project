#pragma once

class GameScene;

class GameNetwork
{
public:
	GameNetwork();
	~GameNetwork();

public:
	void Update();

private:
	template<class T>
	void ProcessSend(PacketID id, const T& packet);
	void ProcessRecv();
	
	// Send Packet
	void SendUpdateObjectStatePacket(int id, ObjectType type, ObjectState state);
	void SendUpdateDirPacket(int id, ObjectType type, Dir dir);
	void SendMovePacket(int id, ObjectType type, Vertex pos);
	void SendCollisionPacket(CollisionType c_type, int id1, ObjectType type1, Vertex pos1, int id2, ObjectType type2, Vertex pos2);
	void SendUseItemPacket(int id, ObjectType itemType);
	void SendStayGamePacket(int id);
	void SendEndGamePacket(int id);

public:
	template <class T>
	std::vector<char> CreatePacket(PacketID id, T& packet);

public:
	void SetGameScene(GameScene* scene) { _gameScene = scene; }

private:
	SOCKET _socket;

	fd_set _readSet{};
	fd_set _writeSet{};

	GameScene* _gameScene{};
};
