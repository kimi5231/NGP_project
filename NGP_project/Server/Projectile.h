#pragma once
#include "GameObject.h"

class Projectile : public GameObject
{
public:
	Projectile(Dir dir, const Vertex& pos);
public:
	virtual void Update();
private:
	virtual void Move();
};