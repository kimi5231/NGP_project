#pragma once
#include "GameObject.h"

class Merchant : public GameObject
{
	using Parent = GameObject;

public:
	Merchant(HINSTANCE hInst);
	virtual ~Merchant() {};

public:
	virtual void Init(HINSTANCE hInst);
	virtual void Update();
	virtual void Render(HDC hDC, HDC srcDC);
};