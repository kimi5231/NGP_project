#pragma once

class GameScene;

class GameNetwork
{
public:
	GameNetwork(char* ip);
	~GameNetwork();

public:
	void Update();

public:
	template<class T>
	void ProcessSend(PacketID id, const T& packet);
	void ProcessRecv();
	
	// Send Packet
	void SendMovePacket(int id, ObjectType type, Vertex pos, Dir dir, ObjectState state);
	void SendCreateProjectilePacket(int id, Vertex pos, Dir dir);
	void SendKickBombPacket(int id, Dir dir);
	void SendCollisionPacket(CollisionType c_type, int id1, ObjectType type1, Vertex pos1, int id2, ObjectType type2, Vertex pos2);
	void SendUseItemPacket(int id, ObjectType player, ItemType itemType);
	void SendStayGamePacket(int id);
	void SendEndGamePacket(int id);

	// Recv 동작
	void RecvAddObject(S_AddObject_Packet addObjectPacket);
	void RecvRemoveObject(S_RemoveObject_Packet removeObjectPacket);
	void RecvUpdateObjectState(S_UpdateObjectState_Packet updateObjectStatePacket);
	void RecvUpdateDir(S_UpdateDir_Packet updateDirPacket);
	void RecvMove(S_Move_Packet movePacket);
	void RecvChangeNextStage(S_ChangeNextStage_Packet changeNextStagePacket);
	void RecvCollisionResult(S_CollisionResult_Packet collisionResultPacket);
	void RecvMonsterDamaged(S_MonsterDamaged_Packet monsterDamagedPacket);
	void RecvItemUseResult(S_ItemUseResult_Packet itemUseResultPacket);
	void RecvUpdateTimer(S_UpdateTimer_Packet updateTimerPacket);
	void RecvGetItem(S_GetItem_Packet getItemPacket);
	void RecvEndGame();

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
