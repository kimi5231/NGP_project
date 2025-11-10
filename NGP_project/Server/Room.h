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

private:
	std::vector<GameObjectRef> _objects;
};