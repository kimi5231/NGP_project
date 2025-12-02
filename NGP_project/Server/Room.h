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
	void RemoveObject(ObjectType type, int id);

public:
	std::unordered_map<int, PlayerRef>& GetPlayers() { return _players; }
	GameObjectRef GetObject(ObjectType type, int id);
	int GetPlayerCount() { return _playerCount; }
	void SetRoomState(RoomState state) { _state = state; }
	int GetTimer() { return _timer; }
private:
	//std::unordered_map<int, GameObjectRef> _objects;
	std::unordered_map<int, PlayerRef> _players;
	std::unordered_map<int, MonsterRef> _monsters;
	std::unordered_map<int, ItemRef> _items;
	std::unordered_map<int, ProjectileRef> _projectiles;
	int _curStage{ 1 };

	int _generateID;
	int _playerCount;
	int _monsterCount;

	RoomState _state;

	int _timer;

	CRITICAL_SECTION _objectCS;
};