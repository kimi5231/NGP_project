#pragma once

class Monster;

class RespawnMonster : public Monster
{
public:
	RespawnMonster(HINSTANCE hInst);

	virtual void Init(HINSTANCE hInst) override;
};

