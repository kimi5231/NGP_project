#pragma once

class GameObject;

class Monster : public GameObject
{
public:
	Monster();

	virtual void Init() override;

	virtual void Left();
	virtual void Right();
	virtual void Up();
	virtual void Down();
};

