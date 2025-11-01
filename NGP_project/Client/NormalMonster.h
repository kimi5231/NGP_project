#pragma once
#include "Monster.h"

class NormalMonster : public Monster
{
public:
	NormalMonster();
	virtual ~NormalMonster() {};

	virtual void FindTarget(GameObject* other) override;
};