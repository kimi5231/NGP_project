#pragma once
#include "Monster.h"

class RespawnMonster : public Monster
{
public:
	RespawnMonster();
	virtual ~RespawnMonster() {};

	void Update(GameObject* other) override;
	bool UseSkill() override;
	void Damaged(int damage) override;
	virtual void FindTarget(GameObject* other) override;
private:
	int _respawnCnt{};
};