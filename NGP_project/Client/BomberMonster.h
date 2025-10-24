#pragma once

class Monster;

class BomberMonster : public Monster
{
public: 
	BomberMonster(HINSTANCE hInst);

	virtual void Init(HINSTANCE hInst) override;
};

