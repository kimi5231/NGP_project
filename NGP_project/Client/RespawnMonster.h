#pragma once
#include "Monster.h"

class RespawnMonster : public Monster
{
public:
	RespawnMonster();
	virtual ~RespawnMonster() {};

	virtual void FindTarget(GameObject* other) override;
};