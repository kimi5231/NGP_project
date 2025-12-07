#pragma once
#include "GameObject.h"

class BombObject;

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

	void ChangeNextStage();
	void ClearStage();
	void EndGame();

public:
	std::unordered_map<int, PlayerRef>& GetPlayers() { return _players; }
	std::unordered_map<int, MonsterRef>& GetMonsters() { return _monsters; }
	GameObjectRef GetObject(ObjectType type, int id);
	RoomState GetRoomState() { return _state; }
	void SetRoomState(RoomState state) { _state = state; }
	int GetTimer() { return _timer; }

private:
	std::unordered_map<int, PlayerRef> _players;
	std::unordered_map<int, MonsterRef> _monsters;
	std::unordered_map<int, ItemRef> _items;
	std::unordered_map<int, ProjectileRef> _projectiles;
	std::unordered_map<int, BombRef> _bombs;
	std::unordered_map<int, GameObjectRef> _obstacles;

	int _curStage{ 4 };

	int _generateID;
	int _playerCount;
	int _monsterCount;

	RoomState _state;

	int _timer;
};