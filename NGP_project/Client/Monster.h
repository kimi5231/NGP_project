#pragma once
#include "GameObject.h"

class Monster : public GameObject
{
public:
	Monster();
	Monster(ObjectState state);
	virtual ~Monster() {};
public:
	void SetDirAndFrame(Dir dir);
};