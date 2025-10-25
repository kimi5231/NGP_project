#pragma once
#include "GameObject.h"

class Player : public GameObject 
{
public:
	Player();
	virtual ~Player() {};

public:
	virtual void Update();
	virtual void Render(HDC hdc, HDC srcDC);

public:
	// 나중에 Command 패턴 적용
	virtual void Left();
	virtual void Right();
	virtual void Up();
	virtual void Down();
};

