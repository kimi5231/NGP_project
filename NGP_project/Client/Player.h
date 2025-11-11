#pragma once
#include "GameObject.h"

class Player : public GameObject 
{
public:
	Player();
public:
	virtual void Update();
	bool IsCanShoot();
public:
	virtual void Left();
	virtual void Right();
	virtual void Up();
	virtual void Down();
private:
	int _bulletCnt{};
};

