#pragma once
#include "GameObject.h"

class Player : public GameObject {
public:
	Player(HINSTANCE hInst);

	virtual void Init(HINSTANCE hInst) override;

	// 나중에 Command 패턴 적용
	void Left();
	void Right();
	void Up();
	void Down();

	void ResetCurFrame();
private:

};

