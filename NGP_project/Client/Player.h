#pragma once
#include "GameObject.h"

class Player : public GameObject {
public:
	Player(HINSTANCE hInst);

	virtual void Init(HINSTANCE hInst) override;
private:

};

