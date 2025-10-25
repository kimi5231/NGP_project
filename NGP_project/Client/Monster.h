#pragma once

class GameObject;

class Monster : public GameObject
{
public:
	Monster(HINSTANCE hInst);

	virtual void Init(HINSTANCE hInst) override;

	virtual void Left();
	virtual void Right();
	virtual void Up();
	virtual void Down();
};

