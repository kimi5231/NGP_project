#pragma once

class Monster;

class TankMonster : public Monster
{
public:
	TankMonster(HINSTANCE hInst);

	virtual void Init(HINSTANCE hInst) override;
};

