#pragma once
#include "GameObject.h"

class Monster : public GameObject
{
public:
	Monster();
	virtual ~Monster() {};
public:
	virtual void Move() override;

	virtual void Update() override;
};