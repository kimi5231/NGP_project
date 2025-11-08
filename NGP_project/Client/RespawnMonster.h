#pragma once
#include "Monster.h"

class RespawnMonster : public Monster
{
public:
	RespawnMonster();
	virtual ~RespawnMonster() {};

	bool UseSkill() override;
	virtual void FindTarget(GameObject* other) override;
};