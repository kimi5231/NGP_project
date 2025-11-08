#pragma once
#include "GameObject.h"

class Projectile : public GameObject
{
public:
	Projectile(Dir dir, const Vertex& pos);
public:
	virtual void Update();
	virtual void Render(HDC hdc, HDC srcDC);

	int GetDamage() const { return _damage; }
private:
	virtual void Move();

	int _damage;
};