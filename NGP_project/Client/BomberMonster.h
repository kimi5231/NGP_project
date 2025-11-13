#pragma once
#include "Monster.h"

class BomberMonster : public Monster
{
public: 
	BomberMonster();
	virtual ~BomberMonster() {};

	bool UseSkill() override;
};

