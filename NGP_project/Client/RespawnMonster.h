#pragma once

class Monster;

class RespawnMonster : public Monster
{
public:
	RespawnMonster();

	virtual void Init() override;
};

