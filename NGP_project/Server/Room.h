#pragma once
#include "GameObject.h"

class Room
{
public:
	Room();
	~Room();

public:
	void Update();
	void InitObstalce();
	void InitStage();
	void SpawnMonster();

public:
	GameObjectRef AddObject(ObjectType type);
	S_RemoveObject_Packet RemoveObject(int id);

public:
	std::vector<GameObjectRef>& GetObjects() { return _objects; }
	int GetPlayerCount() { return _playerCount; }
	void SetRoomState(RoomState state) { _state = state; }
	int GetTimer() { return _timer; }

private:
	std::vector<GameObjectRef> _objects;
	int _curStage{ 1 };

	CRITICAL_SECTION _cs;

	int _generateID;
	int _playerCount;

	RoomState _state;

	int _timer;
};