#pragma once
#include "GameObject.h"

class Player : public GameObject 
{
public:
	Player();
public:
	virtual void Update();
public:
	virtual void Left();
	virtual void Right();
	virtual void Up();
	virtual void Down();

};

