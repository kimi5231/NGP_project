#pragma once
#include "GameObject.h"

class Projectile : public GameObject
{
public:
	Projectile(Dir dir);
	virtual ~Projectile() {};

public:
	virtual void Update();
	virtual void Render(HDC hdc, HDC srcDC);

private:
	virtual void Move();
};