#pragma once
#include "GameObject.h"

class Projectile : public GameObject
{
public:
	Projectile();
public:
	virtual void Render(HDC hdc, HDC srcDC);
};