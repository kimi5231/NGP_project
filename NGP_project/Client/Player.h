#pragma once
#include "GameObject.h"

class Player : public GameObject {
public:
	Player();

	virtual void Init() override;

	// 나중에 Command 패턴 적용
	void Left();
	void Right();
	void Up();
	void Down();
private:

};

