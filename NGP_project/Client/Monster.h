#pragma once
#include "GameObject.h"

class Monster : public GameObject
{
public:
	Monster();
	virtual ~Monster() {};
public:
	virtual void Left();
	virtual void Right();
	virtual void Up();
	virtual void Down();
	virtual void Move() override;

	virtual void Update() override;
};