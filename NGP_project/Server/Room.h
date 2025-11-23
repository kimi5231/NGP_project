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

public:
	std::vector<GameObjectRef>& GetObjects() { return _objects; }

private:
	std::vector<GameObjectRef> _objects;
	int _curStage{ 1 };

	CRITICAL_SECTION _cs;

	int _generateID;
};