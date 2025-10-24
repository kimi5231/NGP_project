#pragma once

class Monster;

class NormalMonster : public Monster
{
public:
	NormalMonster(HINSTANCE hInst);

	virtual void Init(HINSTANCE hInst) override;
};

