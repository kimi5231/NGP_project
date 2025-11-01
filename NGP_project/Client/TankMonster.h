#pragma once
#include "Monster.h"

class TankMonster : public Monster
{
public:
	TankMonster();
	virtual ~TankMonster() {};

	virtual void FindTarget(GameObject* other) override;
};