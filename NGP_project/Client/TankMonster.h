#pragma once

class Monster;

class TankMonster : public Monster
{
public:
	TankMonster();

	virtual void Init() override;
};

