#pragma once
#include "GameObject.h"

class Merchant : public GameObject
{
	using Parent = GameObject;

public:
	Merchant();
	virtual ~Merchant() {};

public:
	virtual void Init();
	virtual void Update();
	virtual void Render(HDC hDC, HDC srcDC);
};