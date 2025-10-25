#pragma once

class Monster;

class NormalMonster : public Monster
{
public:
	NormalMonster();

	virtual void Init() override;
};

