#pragma once
#include "GameObject.h"

class Room
{
public:
	Room();
	~Room();

public:
	void Update();

public:
	GameObjectRef AddObject(ObjectType type);

public:
	std::vector<GameObjectRef>& GetObjects() { return _objects; }

private:
	std::vector<GameObjectRef> _objects;

	CRITICAL_SECTION _cs;

	int _generateID;
};