#pragma once
#include "GameObject.h"

class Projectile : public GameObject, public std::enable_shared_from_this<Projectile>
{
public:
	Projectile(Dir dir, const Vertex& pos);
public:
	virtual void Update();
private:
	virtual bool Move();
};