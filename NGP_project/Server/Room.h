#pragma once
#include "GameObject.h"

class Room
{
public:
	Room();
	~Room();

public:
	void Update();

private:
	std::vector<GameObjectRef> _objects;
};