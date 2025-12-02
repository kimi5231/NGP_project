#pragma once
#include "GameObject.h"

class Room
{
public:
	Room();
	~Room();

public:
	void Update();
	void SpawnMonster();

public:
	GameObjectRef AddObject(ObjectType type, Vertex pos = {-1, -1}, Dir dir = Dir::None);
	void RemoveObject(int id);
public:
	std::unordered_map<int, GameObjectRef>& GetObjects() { return _objects; }
	GameObjectRef GetObject(int id) { return _objects[id]; }
	int GetPlayerCount() { return _playerCount; }
	void SetRoomState(RoomState state) { _state = state; }
	int GetTimer() { return _timer; }
private:
	std::unordered_map<int, GameObjectRef> _objects;
	int _curStage{ 1 };

	int _generateID;
	int _playerCount;

	RoomState _state;

	int _timer;

	CRITICAL_SECTION _cs;
};